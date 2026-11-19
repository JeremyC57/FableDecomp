// Behaviour test for CEnginePrimitiveRenderer::LoadNewScene (0x00B328C0), a portable model. The preload radius is
// (min - max) * progress + max around the camera; the preload mipmap tolerance is floor(progress * max tolerance); the mipmap size
// multiplier is mipmap quality * LOD screen size and the FOV-adjusted size is the LOD screen size; with video options the two LOD error
// tolerances come from the mesh-detail def. Flags start at 1 unless the params' flag 2 is set; each scene preloads by camera with
// flags | 2 and by sphere with flags; then flags | 4 is used for the shadow pass after the lighting manager's shadow update. The result
// is the OR of every preload.
#include <stdio.h>
#include <math.h>
#include <vector>
struct Scene { bool cam_ret, sphere_ret, clip_ret; unsigned long cam_flags, sphere_flags, clip_flags; float sphere_radius; int clip_calls; };
struct Def { float lod, char_lod; };
struct Renderer {
    float max_r, min_r, max_tol, mip_quality, lod_size, lod_fov, mip_mult, lod_err, char_lod_err; long preload_tol;
    std::vector<Scene*> scenes; int shadow_updates;
    bool LoadNewScene(float progress, unsigned long pflags, const int* mesh_detail, const Def* defs)
    {
        float radius = (min_r - max_r) * progress + max_r;
        preload_tol = (long)floor(progress * max_tol);
        mip_mult = mip_quality * lod_size;
        lod_fov = lod_size;
        if (mesh_detail) { lod_err = defs[*mesh_detail].lod; char_lod_err = defs[*mesh_detail].char_lod; }
        unsigned long flags = 0;
        if (!(pflags & 2)) flags |= 1;
        bool loading = false;
        for (size_t i = 0; i < scenes.size(); ++i) {
            scenes[i]->cam_flags = flags | 2; loading |= scenes[i]->cam_ret;
            scenes[i]->sphere_flags = flags; scenes[i]->sphere_radius = radius; loading |= scenes[i]->sphere_ret;
        }
        flags |= 4;
        ++shadow_updates;
        for (size_t i = 0; i < scenes.size(); ++i) { scenes[i]->clip_flags = flags; ++scenes[i]->clip_calls; loading |= scenes[i]->clip_ret; }
        return loading;
    }
};
int main()
{
    int bad = 0;
    Scene a = { false, false, false, 0, 0, 0, 0, 0 }, b = { false, false, false, 0, 0, 0, 0, 0 };
    Renderer r = { 100.0f, 20.0f, 7.0f, 0.5f, 600.0f, 0, 0, 0, 0, 0 };
    r.scenes.push_back(&a); r.scenes.push_back(&b); r.shadow_updates = 0;
    Def defs[3] = { { 1, 2 }, { 3, 4 }, { 5, 6 } };
    int detail = 2;
    if (r.LoadNewScene(0.25f, 0, &detail, defs)) ++bad;                       // nothing loading
    if (a.sphere_radius != 80.0f || r.preload_tol != 1 || r.mip_mult != 300.0f || r.lod_fov != 600.0f || r.lod_err != 5.0f || r.char_lod_err != 6.0f) ++bad;
    if (a.cam_flags != 3 || a.sphere_flags != 1 || a.clip_flags != 5 || b.clip_calls != 1 || r.shadow_updates != 1) ++bad;
    b.clip_ret = true;
    if (!r.LoadNewScene(1.0f, 2, 0, defs)) ++bad;                             // shadow pass still loading; flag 2 set
    if (a.cam_flags != 2 || a.sphere_flags != 0 || a.clip_flags != 4 || a.sphere_radius != 20.0f || r.preload_tol != 7 || r.lod_err != 5.0f) ++bad;
    b.clip_ret = false; a.sphere_ret = true;
    if (!r.LoadNewScene(0.0f, 0, 0, defs) || r.preload_tol != 0 || a.sphere_radius != 100.0f) ++bad;
    printf(bad ? "00b328c0_TEST FAIL\n" : "00b328c0_TEST PASS\n");
    return bad ? 1 : 0;
}
