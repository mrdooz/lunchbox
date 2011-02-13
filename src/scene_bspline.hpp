#pragma once

#include "resource_manager.hpp"
#include "scene.hpp"
#include <D3DX10math.h>
#include <celsus/dynamic_vb.hpp>
#include <celsus/vertex_types.hpp>

class Mesh2;
struct Material;

struct BSpline
{

};

class SceneBSpline : public Scene
{
public:
  SceneBSpline();
  virtual ~SceneBSpline();

	virtual bool init();
	virtual bool close();
	virtual bool render();

private:

  void update(float t, float dt, int num_ticks, float a);

  void extrude(const BSpline& b);
  void extrude2(const BSpline& b);

  bool effect_loaded(EffectWrapper *effect);
	bool load_states(const string2& filename);

  //std::vector<D3DXVECTOR3> _cur_points;
  //float _progress;
  //AppendableVb<PosNormal> _verts;
  //DynamicVb<PosNormal> _tmp_verts;
  CComPtr<ID3D11InputLayout> _layout;
  std::auto_ptr<EffectWrapper> _effect;
  //int _vertex_count;

  //D3DXVECTOR3 _up;
  //D3DXVECTOR3 _cur_top;
  //float _last_update;
  //float _angle;

	StaticBuffer _vb;
	StaticBuffer _ib;
};

