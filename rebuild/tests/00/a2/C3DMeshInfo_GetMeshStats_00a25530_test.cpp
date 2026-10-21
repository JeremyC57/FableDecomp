// Behaviour test for C3DMeshInfo::GetMeshStats at 0x00A25530: stats-slot hit, loaded-LOD hit, and the synchronous load path.
#include <cstdio>
#include <cstring>
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); }
struct LOD { int mesh; int refs; int used; int dropOnBuild; };
struct Info {
    LOD* lods[5];            // [4] = MeshStats
    unsigned int sizes[4];
    LOD loaded;
    int Get()
    {
        bool load = true;
        if (lods[4]) { if (lods[4]->mesh) { Log("T4;"); return lods[4]->mesh; } load = false; }
        for (unsigned int i = 0; i < 4 && sizes[i]; ++i) {
            if (lods[i]) {
                if (lods[i]->mesh) { Log("T;"); lods[i]->used = 1; return lods[i]->mesh; }
                load = false;
            }
        }
        if (load) { Log("L;"); lods[4] = &loaded; }
        LOD* info = lods[4];
        if (!info) { int idx = -1; for (unsigned int i = 0; i < 4 && sizes[i]; ++i) if (lods[i]) { idx = i; break; } info = lods[idx]; }
        Log("W;");
        ++info->refs;
        info->mesh = 42;   // Build
        if (info->dropOnBuild) info->refs = 1;
        if (--info->refs == 0) Log("R;");
        info->used = 1;
        return info->mesh;
    }
};
int main()
{
    int ok = 1;
    LOD s = { 7, 1, 0, 0 };
    Info a = { { 0, 0, 0, 0, &s }, { 1, 1, 0, 0 }, { 0, 1, 0, 0 } };
    ok &= a.Get() == 7 && std::strcmp(g_log, "T4;") == 0; g_log[0] = 0;
    LOD l1 = { 9, 1, 0, 0 };
    Info b = { { 0, &l1, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 0, 0 } };
    ok &= b.Get() == 9 && l1.used == 1 && std::strcmp(g_log, "T;") == 0; g_log[0] = 0;
    Info c = { { 0, 0, 0, 0, 0 }, { 1, 0, 0, 0 }, { 0, 1, 0, 0 } };
    ok &= c.Get() == 42 && c.loaded.used == 1 && std::strcmp(g_log, "L;W;") == 0;
    std::printf(ok ? "00a25530_TEST PASS\n" : "00a25530_TEST FAIL\n");
    return ok ? 0 : 1;
}