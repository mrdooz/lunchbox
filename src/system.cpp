#include "stdafx.h"
#include "system.hpp"

/*
  TODO: load a shindig.conf, and a local shindig.user.conf
*/

System* System::_instance = nullptr;

System::System()
{
  _debug_host = "192.168.0.17";
}

System::~System()
{
  delete this;
  _instance = nullptr;
}

System& System::instance()
{
	if (_instance == NULL)
		_instance = new System();
	return *_instance;
}

bool System::init()
{
  enum_known_folders();

	return true;
}

bool System::close()
{
	return true;
}

bool System::tick()
{
	return true;
}

void System::enum_known_folders()
{

  char buf[MAX_PATH+1];
  _getcwd(buf, MAX_PATH);
  _working_dir = buf;
  _working_dir += "\\";

  // find the "my documents" and drop box folders
  // See https://cfx.svn.codeplex.com/svn/Visual%20Studio%202008/CppShellKnownFolders/ReadMe.txt
  // for a good description of how this stuff works
  CComPtr<IKnownFolderManager> pkfm;
  RETURN_ON_FAIL_VOID(CoCreateInstance(CLSID_KnownFolderManager, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pkfm)), LOG_WARNING_LN);
  KNOWNFOLDERID id;
  RETURN_ON_FAIL_VOID(pkfm->FolderIdFromCsidl(CSIDL_MYDOCUMENTS, &id), LOG_WARNING_LN);
  CComPtr<IKnownFolder> k;
  RETURN_ON_FAIL_VOID(pkfm->GetFolder(id, &k), LOG_WARNING_LN);
  KNOWNFOLDER_DEFINITION kfd;
  RETURN_ON_FAIL_VOID(k->GetFolderDefinition(&kfd), LOG_WARNING_LN);
  PWSTR pszPath = NULL;
  if (SUCCEEDED(k->GetPath(0, &pszPath))) {
    char* str = NULL;
    UnicodeToAnsi(pszPath, &str);
    _my_documents = str;
    _dropbox = _my_documents + "\\My DropBox\\";
    if( !(GetFileAttributesA(_dropbox) & FILE_ATTRIBUTE_DIRECTORY)) {
      _dropbox = "";
    }

    free(str);
    CoTaskMemFree(pszPath);
  }
  FreeKnownFolderDefinitionFields(&kfd);
}

string2 System::convert_path(const string2& str, DirTag tag)
{
  string2 res;
  switch(tag) {
    case kDirRelative:
      res = working_dir() + str;
      break;
    case kDirDropBox:
      res = dropbox() + str;
      break;
    case kDirAbsolute:
      res = str;
      break;
		case kDirCustom:
			// search the custom dirs until we find a matching file..
			for (int i = 0; i < (int)_custom_dirs.size(); ++i) {
				string2 t = _custom_dirs[i] + str;
				if (file_exists(t))
					return t;
			}
			break;
  }
  return res;
}

void System::add_error_message(const char* fmt, ...)
{
  va_list arg;
  va_start(arg, fmt);

  const int len = _vscprintf(fmt, arg) + 1;

  char* buf = (char*)_alloca(len);
  vsprintf_s(buf, len, fmt, arg);
  va_end(arg);
  _error_msg = buf;

  LOG_ERROR_LN(buf);
}

void System::add_custom_dir(const string2& dir)
{
	_custom_dirs.push_back(dir);
}
