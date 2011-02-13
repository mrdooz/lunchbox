#include "stdafx.h"
#include "scene_bspline.hpp"
#include "app.hpp"
#include "system.hpp"
#include "geometry.hpp"

using namespace std;
using namespace fastdelegate;

void SceneBSpline::extrude(const BSpline& bezier)
{
/*
	const int splits = 20;
	float inc = 1 / (float)(splits - 1);

	const uint32_t spins = 20;

	static bool first_point = true;

	static D3DXVECTOR3 prev_binormal;
	const int32_t num_segments = bezier.curves.size();

	vector<PosNormal> verts;
	verts.reserve(num_segments*splits*spins);

	const float max_t = (float)num_segments;
	const float inc_t = 1.0f / splits;
	float t = 0;
	while (t <= max_t) {
		D3DXVECTOR3 cur = bezier.interpolate(t);
		static D3DXVECTOR3 tangent = vec3_normalize(bezier.diff1(t));

		D3DXVECTOR3 binormal;
		D3DXVECTOR3 normal;
		if (first_point) {
			first_point = false;
			normal = vec3_cross(tangent, bezier.diff2(t));
			// if the curvature is 0, choose any normal perpedictular to the tangent
			normal = vec3_normalize(D3DXVec3LengthSq(&normal) < 0.0001f ? find_orthogonal(tangent) : vec3_cross(normal, tangent));
			binormal = vec3_cross(tangent, normal);
		} else {
			// Ken Sloan's method to propagate the reference frame
			normal = vec3_cross(prev_binormal, tangent);
			binormal = vec3_cross(tangent, normal);
		}

		prev_binormal = binormal;

		const float delta_angle = 2 * (float)D3DX_PI / (float)(spins - 1);
		float cur_angle = 0;
		D3DXVECTOR3 tmp;
		D3DXVECTOR3 tmp2;
		D3DXMATRIX mtx_rot2(kMtxId);

		D3DXVECTOR3 scaled_s = normal;

		for (uint32_t k = 0; k < spins; ++k, cur_angle += delta_angle) {
			D3DXMatrixRotationAxis(&mtx_rot2, &tangent, cur_angle);
			D3DXVec3TransformCoord(&tmp, &scaled_s, &mtx_rot2);
			D3DXVec3TransformNormal(&tmp2, &normal, &mtx_rot2);
			verts.push_back(PosNormal(tmp + cur, tmp2));
		}
		t += inc_t;
	}

	PosNormal *vtx = _verts.map();
	const int segments = verts.size() / spins - 1;
	for (int i = 0; i < segments; ++i) {
		for (int j = 0; j < spins; ++j) {
			const PosNormal& v0 = verts[(i+0)*spins+j];
			const PosNormal& v1 = verts[(i+1)*spins+j];
			const PosNormal& v2 = verts[(i+1)*spins+((j+1)%spins)];
			const PosNormal& v3 = verts[(i+0)*spins+((j+1)%spins)];
			*vtx++ = v0;
			*vtx++ = v2;
			*vtx++ = v1;

			*vtx++ = v0;
			*vtx++ = v3;
			*vtx++ = v2;
		}
	}
	_verts.unmap(vtx);
*/
}

void SceneBSpline::extrude2(const BSpline& bezier)
{
/*
	// uses the progress to determine how much to extrude

	const int num_segments = bezier.curves.size();
	const int splits = 20;

	const uint32_t spins = 20;

	static bool first_point = true;

	static D3DXVECTOR3 prev_binormal;

	vector<PosNormal> verts;
	verts.reserve(num_segments*splits*spins);

	const float max_t = _progress * num_segments;
	const float inc_t = 1.0f / splits;
	float t = 0;
	while (t <= max_t) {
		D3DXVECTOR3 cur = bezier.interpolate(t);
		static D3DXVECTOR3 tangent = vec3_normalize(bezier.diff1(t));

		D3DXVECTOR3 binormal;
		D3DXVECTOR3 normal;
		if (first_point) {
			first_point = false;
			normal = vec3_cross(tangent, bezier.diff2(t));
			// if the curvature is 0, choose any normal perpedictular to the tangent
			normal = vec3_normalize(D3DXVec3LengthSq(&normal) < 0.0001f ? find_orthogonal(tangent) : vec3_cross(normal, tangent));
			binormal = vec3_cross(tangent, normal);
		} else {
			// Ken Sloan's method to propagate the reference frame
			normal = vec3_cross(prev_binormal, tangent);
			binormal = vec3_cross(tangent, normal);
		}

		prev_binormal = binormal;

		const float delta_angle = 2 * (float)D3DX_PI / (float)(spins - 1);
		float cur_angle = 0;
		D3DXVECTOR3 tmp;
		D3DXVECTOR3 tmp2;
		D3DXMATRIX mtx_rot2(kMtxId);

		D3DXVECTOR3 scaled_s = normal;

		for (uint32_t k = 0; k < spins; ++k, cur_angle += delta_angle) {
			D3DXMatrixRotationAxis(&mtx_rot2, &tangent, cur_angle);
			D3DXVec3TransformCoord(&tmp, &scaled_s, &mtx_rot2);
			D3DXVec3TransformNormal(&tmp2, &normal, &mtx_rot2);
			verts.push_back(PosNormal(tmp + cur, tmp2));
		}
		t += inc_t;
	}

	PosNormal *vtx = _tmp_verts.map();
	const int segments = verts.size() / spins - 1;
	for (int i = 0; i < segments; ++i) {
		for (int j = 0; j < spins; ++j) {
			const PosNormal& v0 = verts[(i+0)*spins+j];
			const PosNormal& v1 = verts[(i+1)*spins+j];
			const PosNormal& v2 = verts[(i+1)*spins+((j+1)%spins)];
			const PosNormal& v3 = verts[(i+0)*spins+((j+1)%spins)];
			*vtx++ = v0;
			*vtx++ = v2;
			*vtx++ = v1;

			*vtx++ = v0;
			*vtx++ = v3;
			*vtx++ = v2;
		}
	}
	_tmp_verts.unmap(vtx);
*/
}

BSpline bezier;

SceneBSpline::SceneBSpline()
{
}

SceneBSpline::~SceneBSpline()
{
}

bool create_buffers(const Triangles &tris, const Vertices &verts, ID3D11Buffer **vb, ID3D11Buffer **ib)
{
	auto *device = Graphics::instance().device();
	if (FAILED(create_static_vertex_buffer(device, verts.size(), sizeof(Vertices::value_type), verts.data(), vb)))
		return false;

	vector<int> v;
	for (size_t i = 0; i < tris.size(); ++i) {
		for (int j = 0; j < 3; ++j)
			v.push_back(tris[i]->i[j]);
	}

	if (FAILED(create_static_index_buffer(device, v.size(), sizeof(vector<int>::value_type), v.data(), ib)))
		return false;

	return true;
}

bool SceneBSpline::init()
{

	vector<D3DXVECTOR3> verts;
	vector<Triangle *> tris;

	create_cube(D3DXVECTOR3(0,0,0), D3DXVECTOR3(10, 10, 10), &tris, &verts);

	RETURN_ON_FAIL_BOOL_E(create_buffers(tris, verts, &_vb.buffer.p, &_ib.buffer.p));
	RETURN_ON_FAIL_BOOL_E(
		ResourceManager::instance().load_shaders(System::instance().convert_path("effects/test_effect6.fx", System::kDirRelative), "vsMain", NULL, "psMain", 
		MakeDelegate(this, &SceneBSpline::effect_loaded)));

	return true;
/*
	auto& s = System::instance();
	auto& r = ResourceManager::instance();

	RETURN_ON_FAIL_BOOL_E(_verts.create(1000000));
	RETURN_ON_FAIL_BOOL_E(_tmp_verts.create(20000));
	RETURN_ON_FAIL_BOOL_E(r.load_shaders(s.convert_path("effects/test_effect6.fx", System::kDirRelative), "vsMain", NULL, "psMain", MakeDelegate(this, &SceneBSpline::effect_loaded)));
	//App::instance().add_update_callback(MakeDelegate(this, &TestEffect7::update), true);


	D3DXMATRIX mtx;
	const float len = 1;
	vector<D3DXVECTOR3> pts;
	D3DXVECTOR3 up(0,1,0), cur_top(0,0,0);
	float angle = 0;
	for (int j = 0; j < 100; ++j) {
		for (int i = 0; i < 4; ++i) {
			// each point is selected as a random point on the circle around
			// the current normal

			D3DXVECTOR3 pt = cur_top + len * _up + D3DXVECTOR3(0.1f, 0, 0);
			//angle += randf(-(float)D3DX_PI/100, (float)D3DX_PI/100);
			D3DXMatrixRotationAxis(&mtx, &up, angle);
			D3DXVec3TransformCoord(&pt, &pt, &mtx);
			pts.push_back(pt);
			up = vec3_normalize(pt - cur_top);
			cur_top = pt;
		}

	}

	//_full_curve = BSpline::from_points(AsArray<D3DXVECTOR3>(pts));

	return true;
*/
}

bool SceneBSpline::close()
{
	//App::instance().add_update_callback(MakeDelegate(this, &TestEffect7::update), false);

	return true;
}

bool SceneBSpline::render()
{
	Graphics& g = Graphics::instance();
	ID3D11Device* device = g.device();
	ID3D11DeviceContext* context = g.context();

	context->OMSetDepthStencilState(g.default_depth_stencil_state(), g.default_stencil_ref());
	context->OMSetBlendState(g.default_blend_state(), g.default_blend_factors(), g.default_sample_mask());
	context->RSSetState(g.default_rasterizer_state());

	D3DXMATRIX mtx;
	D3DXMATRIX view(kMtxId);
	D3DXMATRIX proj(kMtxId);
	D3DXMatrixTranspose(&mtx, &(view * proj));
	_effect->set_vs_variable("mtx", mtx);
	_effect->set_cbuffer();
	_effect->unmap_buffers();

	_effect->set_shaders(context);

	context->IASetInputLayout(_layout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	set_ib(context, _ib);
	set_vb(context, _vb);
	context->Draw(_vb.num_elems, 0);

	return true;
}

bool SceneBSpline::load_states(const string2& filename)
{
	return true;
}

bool SceneBSpline::effect_loaded(EffectWrapper *effect)
{
	_effect.reset(effect);
	return InputDesc(). 
		add("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0).
		//add("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12).
		create(_layout, _effect.get());
}

void SceneBSpline::update(float t, float dt, int num_ticks, float a)
{

#if 0
	_progress = min(1, max(0, (t - _last_update)));
	// every second, add a new curve
	if (t - _last_update > 1.0f) {

		// add the previous bezier segments
		if (!bezier.curves.empty())
			extrude(bezier);

		_last_update = t;
		_progress = 0;

		bezier.curves.clear();
		int ofs = max(0,(int)t-1);
		int stride = 2;
		for (int i = 0; i < stride; ++i)
			bezier.curves.push_back(_full_curve.curves[ofs*stride+i]);
		/*
		D3DXMATRIX mtx;
		const float len = 2;
		_cur_points.clear();
		_cur_points.push_back(_cur_top);
		for (int i = 0; i < 4; ++i) {
		// each point is selected as a random point on the circle around
		// the current normal

		D3DXVECTOR3 pt = _cur_top + len * _up + D3DXVECTOR3(0.1f, 0, 0);
		_angle += randf(-(float)D3DX_PI/100, (float)D3DX_PI/100);
		D3DXMatrixRotationAxis(&mtx, &_up, _angle);
		D3DXVec3TransformCoord(&pt, &pt, &mtx);
		_cur_points.push_back(pt);
		_up = vec3_normalize(pt - _cur_top);
		_cur_top = pt;
		}
		*/
		//bezier = BSpline::from_points(AsArray<D3DXVECTOR3>(_cur_points));
	}
#endif
}
