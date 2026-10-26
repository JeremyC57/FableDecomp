#include <cstdio>

class CEngineCamera { public: int x; };
static float g_d = 0; static void* g_c = 0; static long g_f = -1;
struct QT { unsigned char pad[0x48]; unsigned char Flags; void RenderUpdate(float d, CEngineCamera* c, long f) { g_d = d; g_c = c; g_f = f; } };
struct M { unsigned char pad[0xc]; QT QuadTree; void Render(CEngineCamera* c, float d) { QT* t = &QuadTree; if (t->Flags & 2) t->RenderUpdate(d, c, 0); } };
int main()
{
    CEngineCamera cam; M m; m.QuadTree.Flags = 0; m.Render(&cam, 1.5f); if (g_c) { std::printf("00bddd70_TEST FAIL\n"); return 1; }
    m.QuadTree.Flags = 2; m.Render(&cam, 1.5f); if (g_c != &cam || g_d != 1.5f || g_f != 0) { std::printf("00bddd70_TEST FAIL\n"); return 1; }
    std::printf("00bddd70_TEST PASS\n");
    return 0;
}