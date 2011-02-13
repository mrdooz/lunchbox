#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_

class System
{
public:

  enum DirTag {
    kDirDropBox,
    kDirRelative,
    kDirAbsolute,
		kDirCustom,
  };

	static System& instance();
	bool init();
	bool close();
	bool tick();

  const string2& dropbox() const { return _dropbox; }
  const string2& working_dir() const { return _working_dir; }
  const string2& debug_host() const { return _debug_host; }

	void add_custom_dir(const string2& dir);

  string2 convert_path(const string2& str, DirTag tag);

  void add_error_message(const char* fmt, ...);

  std::string error_message() const;

private:
	DISALLOW_COPY_AND_ASSIGN(System);
	System();
	~System();

  void enum_known_folders();

	DWORD _main_thread_id;
	static System* _instance;

  string2 _my_documents;
  string2 _dropbox;
  string2 _working_dir;
  string2 _debug_host;

  std::string _error_msg;

	std::vector<string2> _custom_dirs;
};

#endif
