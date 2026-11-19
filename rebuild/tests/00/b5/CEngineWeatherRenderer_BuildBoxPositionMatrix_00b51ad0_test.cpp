// Behaviour test for 0x00B51AD0 CEngineWeatherRenderer::BuildBoxPositionMatrix (links the REAL row object: the inline GetBoxCoord and
// the six GFFloatToLongFloor inline-assembly floors run as compiled). The row's class is declared here with its one private member
// (?BuildBoxPositionMatrix@CEngineWeatherRenderer@@AAEXXZ); NEngineInternal::GPCamera (?GPCamera@NEngineInternal@@3PAVCEngineCamera@@A,
// the row's only external) is defined here. Fixtures are raw native-layout images, every offset read by an instruction of the row:
//   camera: position C3DVector at +0x54 (0x00B51ADD / 0x00B51B80 / 0x00B51C2C);
//   renderer: Settings.ParticleBoxZLocked +0x11B (0x00B51C1E), ParticleBoxSpan +0x1D4 / +0x1D8 / +0x1DC (0x00B51B16, 0x00B51BB4,
//   0x00B51C60), ParticleBoxCenter +0x1F0 / +0x1F4 / +0x1F8 (0x00B51B1C, 0x00B51BBA, 0x00B51C66), ParticleBoxPos[81] from +0x1FC.
// Reference model: centre = floor(camera) per axis, then rounded to the nearest multiple of floor(span) (C truncating / and %; ties
// round up: remainder >= span / 2 adds a span), Z only when not Z-locked; box [c_x * 9 + c_y] = ((c_x - 4) * span.X + centre.X,
// (c_y - 4) * span.Y + centre.Y, centre.Z), each component one double-precision expression rounded once to float. Nothing else written.
#include <stdio.h>
#include <string.h>
#include <math.h>

class CEngineCamera;
namespace NEngineInternal { CEngineCamera* GPCamera; }
class CEngineWeatherRenderer;
void RunRow(CEngineWeatherRenderer* r);
class CEngineWeatherRenderer {
    friend void RunRow(CEngineWeatherRenderer* r);
    void BuildBoxPositionMatrix();
};
void RunRow(CEngineWeatherRenderer* r) { r->BuildBoxPositionMatrix(); }

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }
template<class T> static void put(void* base, int off, T v) { memcpy((unsigned char*)base + off, &v, sizeof v); }
template<class T> static T get(const void* base, int off) { T v; memcpy(&v, (const unsigned char*)base + off, sizeof v); return v; }

static union { unsigned char b[0x100]; double align; } camera;
static union { unsigned char b[0x1400]; double align; } renderer;

static long Floor(float x) { return (long)floor((double)x); }
static long Snap(long coord, long span)
{
    long rem = coord % span;
    long out = coord / span * span;
    if (rem >= span / 2) out += span;
    return out;
}

static void Case(float cx, float cy, float cz, float sx, float sy, float sz, bool zlocked, long oldZ, int k)
{
    memset(renderer.b, 0x5C, sizeof renderer.b);
    memset(camera.b, 0x3D, sizeof camera.b);
    put(camera.b, 0x54, cx); put(camera.b, 0x58, cy); put(camera.b, 0x5C, cz);
    NEngineInternal::GPCamera = (CEngineCamera*)(void*)camera.b;
    renderer.b[0x11B] = zlocked ? 1 : 0;
    put(renderer.b, 0x1D4, sx); put(renderer.b, 0x1D8, sy); put(renderer.b, 0x1DC, sz);
    put(renderer.b, 0x1F0, (long)0x11111111); put(renderer.b, 0x1F4, (long)0x22222222); put(renderer.b, 0x1F8, oldZ);
    unsigned char before[sizeof renderer.b]; memcpy(before, renderer.b, sizeof before);
    RunRow((CEngineWeatherRenderer*)(void*)renderer.b);

    long wx = Snap(Floor(cx), Floor(sx)), wy = Snap(Floor(cy), Floor(sy));
    long wz = zlocked ? oldZ : Snap(Floor(cz), Floor(sz));
    check(get<long>(renderer.b, 0x1F0) == wx, "centre X", k);
    check(get<long>(renderer.b, 0x1F4) == wy, "centre Y", k);
    check(get<long>(renderer.b, 0x1F8) == wz, "centre Z (kept when Z-locked)", k);
    bool boxes = true;
    for (long c_x = 0; c_x < 9; ++c_x)
        for (long c_y = 0; c_y < 9; ++c_y) {
            int off = 0x1FC + (int)(c_x * 9 + c_y) * 12;
            float ex = (float)((double)(c_x - 4) * sx + (double)wx);
            float ey = (float)((double)(c_y - 4) * sy + (double)wy);
            float ez = (float)wz;
            if (get<float>(renderer.b, off) != ex || get<float>(renderer.b, off + 4) != ey || get<float>(renderer.b, off + 8) != ez) {
                if (boxes && failures < 40) printf("  box %ld,%ld got %g %g %g want %g %g %g\n", c_x, c_y, get<float>(renderer.b, off),
                                                   get<float>(renderer.b, off + 4), get<float>(renderer.b, off + 8), ex, ey, ez);
                boxes = false;
            }
        }
    check(boxes, "81 box positions", k);
    int endBoxes = 0x1FC + 81 * 12;
    check(memcmp(renderer.b, before, 0x1F0) == 0 && memcmp(renderer.b + endBoxes, before + endBoxes, sizeof before - endBoxes) == 0,
          "nothing outside the centre and the box table written", k);
}

static unsigned long seed = 4242;
static unsigned long Rnd() { seed = seed * 1103515245ul + 12345ul; return (seed >> 8) & 0xffffff; }
static float RndF(float lo, float hi) { return lo + (hi - lo) * (float)(Rnd() % 65536) / 65536.0f; }

int main()
{
    int k = 0;
    Case(103.7f, -55.2f, 12.9f, 20.0f, 16.0f, 10.0f, false, 7, ++k);        // X 103 -> 100 (rem 3 < 10), Y -56 -> -48 (rem -8 < 8)
    Case(115.0f, 24.0f, -5.5f, 20.0f, 16.0f, 10.0f, false, 7, ++k);          // X 115 -> 120 (rem 15 >= 10), Y 24 -> 32 (rem 8 >= 8: tie up)
    Case(110.0f, 23.99f, 5.0f, 20.0f, 16.0f, 10.0f, false, 7, ++k);          // X rem 10 == half: up; Y floor 23 rem 7: down; Z rem 5: up
    Case(3.0f, 4.0f, 2.5f, 20.5f, 16.9f, 10.9f, false, 7, ++k);              // spans floored (20, 16, 10); exact-integer camera values
    Case(-0.5f, -1.0f, -9.99f, 7.0f, 3.0f, 5.0f, false, 7, ++k);             // negatives: floor -1, -1, -10; truncating % (rem -1, -1, 0)
    Case(-25.0f, -26.0f, 0.0f, 10.0f, 9.0f, 4.0f, false, 7, ++k);            // rem -5 (>= 5? no, -5 < 5) / rem -8
    Case(50.5f, 60.5f, 70.5f, 8.0f, 8.0f, 8.0f, true, 123, ++k);             // Z-locked: centre Z kept, boxes use it
    Case(50.5f, 60.5f, 70.5f, 8.0f, 8.0f, 8.0f, true, -77, ++k);
    Case(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, false, 7, ++k);                 // span 1: everything snaps to itself
    Case(1000.25f, -1000.75f, 333.33f, 2.0f, 3.0f, 2.0f, false, 7, ++k);     // span 2 / 3: half 1
    Case(17.0f, 33.0f, 49.0f, 33.0f, 16.0f, 7.0f, false, 7, ++k);            // odd span: half 16
    for (int r = 0; r < 3000; ++r) {
        float sx = RndF(1.0f, 64.0f), sy = RndF(1.0f, 64.0f), sz = RndF(1.0f, 64.0f);
        Case(RndF(-5000.0f, 5000.0f), RndF(-5000.0f, 5000.0f), RndF(-500.0f, 500.0f), sx, sy, sz, (Rnd() & 3) == 0,
             (long)(Rnd() % 2001) - 1000, 1000 + r);
    }
    if (failures) { printf("00b51ad0_TEST FAIL %d\n", failures); return 1; }
    printf("00b51ad0_TEST PASS (%d targeted, 3000 random)\n", k);
    return 0;
}