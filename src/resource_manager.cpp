#include "stdafx.h"
#include "resource_manager.hpp"
#include "scene.hpp"
#include <celsus/file_utils.hpp>
#include <celsus/path_utils.hpp>
#include <celsus/file_watcher.hpp>
#include <celsus/effect_wrapper.hpp>
#include <celsus/ErrorHandling.hpp>

ResourceManager* ResourceManager::_instance = NULL;

using namespace fastdelegate;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

ResourceManager& ResourceManager::instance()
{
	if (_instance == NULL) {
		_instance = new ResourceManager();
	}

	return *_instance;
}

// This can be removed once I stop being stupid and fix a proper bind
struct Trampoline
{
  typedef std::function<bool(const string2&, int)> Fn;
  Trampoline(const Fn& fn, const int flags) : _fn(fn), _flags(flags) {}

  bool run(const string2& str)
  {
    bool res = _fn(str, _flags);
    return res;
  }
  Fn _fn;
  int _flags;
};

bool ResourceManager::load_shaders(const char *filename, const char *vs, const char *gs, const char *ps, const fnEffectLoaded& fn)
{
	auto f = Path::make_canonical(Path::get_full_path_name(filename));
	const int flags = !!vs * kVertexShader | !!gs * kGeometryShader | !!ps * kPixelShader;
	_shader_callbacks[std::make_pair(f, flags)].push_back(ShaderCallbackData(f, vs, gs, ps, fn));
  auto ff = [this, flags](const char *filename) { return reload_shader(filename, flags); };
	return FileWatcher::instance().add_file_changed(f, ff, true);
}

bool ResourceManager::reload_shader(const char* filename, const int shaders)
{
	// find all the callbacks that use this file
  auto key = std::make_pair(filename, shaders);
  auto it = _shader_callbacks.find(key);
	if (_shader_callbacks.find(key) == _shader_callbacks.end()) {
		return true;
	}

	const std::vector<ShaderCallbackData>& n = it->second;
	for (auto i = n.begin(), e = n.end(); i != e; ++i) {
    EffectWrapper* effect = new EffectWrapper();
		if (!effect->load_shaders(i->filename, 
			shaders & kVertexShader ? i->vs : NULL, 
			shaders & kGeometryShader ? i->gs : NULL, 
			shaders & kPixelShader ? i->ps : NULL))
			return false;

		i->fn(effect);
	}
	return true;
}
