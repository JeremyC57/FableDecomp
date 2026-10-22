// Behaviour test for CEngineInternalPrimitive2DViewport::Render (0x00BE1E70): the camera is switched to the 2D description (viewport
// FOVs, near 0.1 / far 1, Z buffer 0..0.1, origin, Y-flipped rotation) and restored afterwards; an empty viewport box renders nothing;
// otherwise fog is disabled, the display viewport is set and restored, and each child with a registered manager is rendered once.
#include <cstdio>

struct Desc { float hfov, vfov; bool use2d; float nearZ, farZ, zs, ze, px, py, pz, e22; };
struct Box { float tlx, tly, brx, bry; };
struct World {
    Desc camera; int cameraSets; Desc lastSet; int displayViewport; int fog; int renders; int typeRenders; int captureDepth;
};
struct Child { int type; bool present; };

void Render(World& w, const Box& viewport, float hfov, float vfov, bool use2d, const Child* children, int count, const bool* managers)
{
    Desc saved = w.camera;
    {
        Desc desc = saved;
        desc.nearZ = 0.1f; desc.farZ = 1.0f; desc.zs = 0.0f; desc.ze = 0.1f;
        desc.use2d = use2d; desc.hfov = hfov; desc.vfov = vfov;
        desc.px = desc.py = desc.pz = 0.0f; desc.e22 = -1.0f;
        w.camera = desc; w.lastSet = desc; ++w.cameraSets;
    }
    int oldViewport = w.displayViewport;
    if (viewport.brx - viewport.tlx > 0.0f && viewport.bry - viewport.tly > 0.0f) {
        ++w.captureDepth;
        w.fog = 0;
        w.displayViewport = 99;
        for (int i = 0; i < count; ++i)
            if (children[i].present && managers[children[i].type])
                ++w.renders;
        ++w.typeRenders;
        w.displayViewport = oldViewport;
        --w.captureDepth;
    }
    w.camera = saved; ++w.cameraSets;
}

int main()
{
    int ok = 1;
    World w; w.camera.hfov = 1.0f; w.camera.vfov = 0.75f; w.camera.use2d = false; w.camera.nearZ = 5.0f; w.camera.farZ = 900.0f;
    w.camera.zs = 0.0f; w.camera.ze = 1.0f; w.camera.px = 3.0f; w.camera.py = 4.0f; w.camera.pz = 5.0f; w.camera.e22 = 1.0f;
    w.cameraSets = 0; w.displayViewport = 7; w.fog = 1; w.renders = 0; w.typeRenders = 0; w.captureDepth = 0;
    Child children[4] = { { 1, true }, { 2, false }, { 3, true }, { 1, true } };
    bool managers[4] = { false, true, false, true };
    Box box = { 0.0f, 0.0f, 100.0f, 50.0f };
    Render(w, box, 0.5f, 0.25f, true, children, 4, managers);
    if (w.renders != 3 || w.typeRenders != 1 || w.fog != 0 || w.displayViewport != 7 || w.captureDepth != 0) ok = 0;
    if (w.lastSet.nearZ != 0.1f || w.lastSet.farZ != 1.0f || w.lastSet.ze != 0.1f || w.lastSet.e22 != -1.0f || !w.lastSet.use2d) ok = 0;
    if (w.lastSet.hfov != 0.5f || w.lastSet.vfov != 0.25f || w.lastSet.px != 0.0f) ok = 0;
    if (w.camera.nearZ != 5.0f || w.camera.px != 3.0f || w.cameraSets != 2) ok = 0;
    World e = w; e.fog = 1;
    Box empty = { 10.0f, 0.0f, 10.0f, 50.0f };
    Render(e, empty, 0.5f, 0.25f, true, children, 4, managers);
    if (e.renders != 3 || e.typeRenders != 1 || e.fog != 1 || e.cameraSets != 4 || e.camera.farZ != 900.0f) ok = 0;
    std::printf(ok ? "00be1e70_TEST PASS\n" : "00be1e70_TEST FAIL\n");
    return ok ? 0 : 1;
}