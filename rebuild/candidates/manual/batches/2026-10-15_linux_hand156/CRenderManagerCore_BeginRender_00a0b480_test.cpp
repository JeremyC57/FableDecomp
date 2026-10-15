// Behaviour test for 0x00A0B480: BeginRender: refuses a second open frame; otherwise stamps the time, unbinds format/index buffer/stream 0, rolls the counters
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct RMC {
    bool rendering; float lastEnd; unsigned long vertexFormat; void* indexBuffer; int streamResets;
    unsigned long z, nonZ, polys, lastZ, lastNonZ, lastPolys; long tris, verts, calls;
    RMC() : rendering(false), lastEnd(0), vertexFormat(7), indexBuffer((void*)this), streamResets(0),
            z(5), nonZ(6), polys(11), lastZ(0), lastNonZ(0), lastPolys(0), tris(100), verts(300), calls(9) {}
    bool BeginRender(double now)
    {
        if (rendering == false)
        {
            lastEnd = (float)now; vertexFormat = 0; indexBuffer = 0; ++streamResets;
            rendering = true;
            lastZ = z; lastNonZ = nonZ; lastPolys = polys;
            z = 0; nonZ = 0; polys = 0; tris = 0; verts = 0; calls = 0;
            return true;
        }
        return false;
    }
};
static void Run()
{
    RMC r;
    CHECK(r.BeginRender(2.5) == true);
    CHECK(r.rendering && r.lastEnd == 2.5f && r.vertexFormat == 0 && r.indexBuffer == 0 && r.streamResets == 1);
    CHECK(r.lastZ == 5 && r.lastNonZ == 6 && r.lastPolys == 11 && r.z == 0 && r.polys == 0 && r.tris == 0 && r.verts == 0 && r.calls == 0);
    CHECK(r.BeginRender(3.0) == false && r.lastEnd == 2.5f && r.streamResets == 1);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00a0b480_TEST FAIL\n"); return 1; }
    std::printf("00a0b480_TEST PASS\n");
    return 0;
}
