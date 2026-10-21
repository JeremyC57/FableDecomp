#include <cstdio>
// Behaviour copy of CDisplayEngine::CDisplayEngine (0x00434E10): the init block copied, camera at (0,0,4) looking +X, fades reset to opaque black, gamma 1.
struct Colour { unsigned char R, G, B, A; Colour() {} Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { A = a; B = b; G = g; R = r; } };
struct Fade { bool Active; bool FadingIn, FadingOut; long StartFrame, Duration; Colour Col; Fade() { Active = false; Duration = 0; StartFrame = 0; FadingOut = false; FadingIn = false; Col = Colour(0, 0, 0, 255); } };
struct Camera { float px, py, pz, dx, dy, dz, fov; long flags; };
struct Init { int* component; int* defs; int* players; int* world; int* gfx; int* mesh; };
struct Engine {
    unsigned long preload; int* component; int* players; int* defs; int* gfx; int* mesh; int* engine3d; Camera cam; bool drawGame; Fade letterBox; bool prepare; float gamma, oldGamma; Fade screenFade;
    bool locked, drawMem; long debugPage; int* view; long readiness; bool initialFade; float duration, sinceStart; int* world;
    Engine(const Init& i) : preload(1), component(i.component), players(i.players), defs(i.defs), gfx(i.gfx), mesh(i.mesh), engine3d(0), drawGame(true), locked(false), drawMem(false), debugPage(0), view(0), initialFade(false), sinceStart(0.0f)
    { cam.px = 0; cam.py = 0; cam.pz = 4; cam.dx = 1; cam.dy = 0; cam.dz = 0; cam.fov = 0.25f; cam.flags = 0; readiness = 30; duration = -1.0f; world = i.world; oldGamma = 0.0f; prepare = true; gamma = 1.0f; }
};
int main()
{
    int a, b, c, d, e, f; Init i = { &a, &b, &c, &d, &e, &f };
    Engine en(i);
    bool ok = en.preload == 1 && en.component == &a && en.defs == &b && en.players == &c && en.world == &d && en.gfx == &e && en.mesh == &f && en.engine3d == 0
        && en.cam.pz == 4.0f && en.cam.dx == 1.0f && en.cam.fov == 0.25f && en.drawGame && !en.letterBox.Active && en.letterBox.Col.A == 255 && en.letterBox.Col.R == 0
        && !en.screenFade.Active && en.screenFade.Col.A == 255 && en.readiness == 30 && en.duration == -1.0f && en.prepare && en.gamma == 1.0f && en.oldGamma == 0.0f && !en.locked && en.view == 0;
    std::printf(ok ? "00434e10_TEST PASS\n" : "00434e10_TEST FAIL\n");
    return ok ? 0 : 1;
}