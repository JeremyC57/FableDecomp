// Behaviour test for 0x00B52990 CEngineWeatherRenderer::UpdateBoxPositions (links the REAL row object: the six GFFloatToLongFloor
// expansions run as compiled). The row's externals are defined here: NEngineInternal::GPCamera and a recording double for the private
// BuildBoxPositionMatrix 0x00B51AD0 (?BuildBoxPositionMatrix@CEngineWeatherRenderer@@AAEXXZ).
// Fixtures are raw native-layout images, every offset read by an instruction of the row: renderer ParticleBoxLocked +0x11A (0x00B52996),
// ParticleBoxSpan +0x1D4 / +0x1D8 / +0x1DC, ParticleBoxCenter +0x1F0 / +0x1F4 / +0x1F8 (long); camera position +0x54 / +0x58 / +0x5C.
// Reference model (independent): rebuild once when not locked and, with h = floor(span * 0.5) and d = floor(|centre - camera|) per axis,
// d >= h on any axis; nothing in the renderer is written by the row itself.
#include <stdio.h>
#include <string.h>
#include <math.h>

class CEngineCamera;
namespace NEngineInternal { CEngineCamera* GPCamera; }
class CEngineWeatherRenderer {
    void UpdateBoxPositions();
    void BuildBoxPositionMatrix();
    friend void CallUpdate(void* renderer);
};
static int rebuilds; static const void* rebuiltThis;
void CEngineWeatherRenderer::BuildBoxPositionMatrix() { ++rebuilds; rebuiltThis = this; }
void CallUpdate(void* renderer) { ((CEngineWeatherRenderer*)renderer)->UpdateBoxPositions(); }

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }
template<class T> static void put(void* base, int off, T v) { memcpy((unsigned char*)base + off, &v, sizeof v); }

static union { unsigned char b[0x700]; double align; } renderer;
static unsigned char camera[0x80];

struct Case { bool locked; float sx, sy, sz; long cx, cy, cz; float camx, camy, camz; };

static unsigned long seed = 99001;
static unsigned long Rnd() { seed = seed * 1103515245ul + 12345ul; return (seed >> 8) & 0xffffff; }
static float RndF(float lo, float hi) { return lo + (hi - lo) * (float)(Rnd() % 65536) / 65536.0f; }
static long Floor(float x) { return (long)floor((double)x); }

static void Run(const Case& c, int k)
{
    memset(renderer.b, 0x4D, sizeof renderer.b); memset(camera, 0x11, sizeof camera);
    renderer.b[0x11A] = c.locked;
    put(renderer.b, 0x1D4, c.sx); put(renderer.b, 0x1D8, c.sy); put(renderer.b, 0x1DC, c.sz);
    put(renderer.b, 0x1F0, c.cx); put(renderer.b, 0x1F4, c.cy); put(renderer.b, 0x1F8, c.cz);
    put(camera, 0x54, c.camx); put(camera, 0x58, c.camy); put(camera, 0x5C, c.camz);
    NEngineInternal::GPCamera = (CEngineCamera*)(void*)camera;
    unsigned char before[sizeof renderer.b]; memcpy(before, renderer.b, sizeof before);
    unsigned char camBefore[sizeof camera]; memcpy(camBefore, camera, sizeof camBefore);
    rebuilds = 0; rebuiltThis = 0;

    CallUpdate(renderer.b);

    bool want = false;
    if (!c.locked) {
        long hx = Floor(c.sx * 0.5f), hy = Floor(c.sy * 0.5f), hz = Floor(c.sz * 0.5f);
        long dx = Floor((float)fabs((float)c.cx - c.camx)), dy = Floor((float)fabs((float)c.cy - c.camy)), dz = Floor((float)fabs((float)c.cz - c.camz));
        want = dx >= hx || dy >= hy || dz >= hz;
    }
    check(rebuilds == (want ? 1 : 0), "rebuild exactly when unlocked and moved half a span on an axis", k);
    check(!want || rebuiltThis == renderer.b, "rebuild called on this", k);
    check(memcmp(before, renderer.b, sizeof before) == 0 && memcmp(camBefore, camera, sizeof camBefore) == 0, "nothing written", k);
}

int main()
{
    int k = 0;
    Case base = { false, 20.0f, 16.0f, 10.0f, 100, 50, 10, 100.0f, 50.0f, 10.0f };
    Case c;
    c = base; Run(c, ++k);                                   // centred: no rebuild
    c = base; c.camx = 110.0f; Run(c, ++k);                  // X exactly half a span: 10 >= 10
    c = base; c.camx = 109.9f; Run(c, ++k);                  // X floor 9 < 10
    c = base; c.camx = 90.0f; Run(c, ++k);                   // negative side
    c = base; c.camy = 41.9f; Run(c, ++k);                   // Y floor 8 >= 8
    c = base; c.camy = 42.1f; Run(c, ++k);                   // Y floor 7 < 8
    c = base; c.camz = 15.0f; Run(c, ++k);                   // Z 5 >= 5
    c = base; c.camz = 14.9f; Run(c, ++k);                   // Z 4 < 5
    c = base; c.sz = 11.0f; c.camz = 15.0f; Run(c, ++k);     // floor(5.5) = 5: 5 >= 5
    c = base; c.sz = 9.0f; c.camz = 14.0f; Run(c, ++k);      // floor(4.5) = 4: 4 >= 4
    c = base; c.camx = 500.0f; c.locked = true; Run(c, ++k); // locked: never
    c = base; c.cx = -100; c.camx = -110.5f; Run(c, ++k);    // negative centre
    c = base; c.sx = 0.5f; Run(c, ++k);                      // half span floors to 0: always
    for (int r = 0; r < 3000; ++r) {
        Case q;
        q.locked = (Rnd() & 7) == 0;
        q.sx = RndF(0.5f, 64.0f); q.sy = RndF(0.5f, 64.0f); q.sz = RndF(0.5f, 64.0f);
        q.cx = (long)(Rnd() % 2001) - 1000; q.cy = (long)(Rnd() % 2001) - 1000; q.cz = (long)(Rnd() % 201) - 100;
        q.camx = (float)q.cx + RndF(-40.0f, 40.0f); q.camy = (float)q.cy + RndF(-40.0f, 40.0f); q.camz = (float)q.cz + RndF(-40.0f, 40.0f);
        Run(q, 1000 + r);
    }
    if (failures) { printf("00b52990_TEST FAIL %d\n", failures); return 1; }
    printf("00b52990_TEST PASS (%d targeted, 3000 random)\n", k);
    return 0;
}