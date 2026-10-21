// behaviour test for 00b68610 CEngineLandscapeRenderer::FrameStart
#include <cstdio>
#include <vector>
static int g_log[64]; static int g_n;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
struct StateMgr {
    void* Stack[16]; long Size; unsigned long Mask; int dummy; int restores;
    void Create() { long& size = Size; Stack[size] = &dummy; Size++; Mask <<= 1; }
    void Restore() { --Size; Mask >>= 1; ++restores; }
};
struct Map { int id; double lastTime; void LOD(double t) { lastTime = t; Log(100 + id); } void Tess() { Log(200 + id); } void Fore() { Log(300 + id); } };
struct Renderer {
    bool Enabled; int mode; void* cur5a0[2]; void* cur544[3]; int a[3], b[3], c[4];
    bool lod, tess; float offset; int memFrames;
    void FrameStart(bool update, std::vector<Map*>& maps, StateMgr& sm, double timer)
    {
        if (!Enabled) return;
        if (mode == 2) { cur5a0[0] = &c[2]; cur5a0[1] = &c[3]; cur544[0] = &b[0]; cur544[1] = &b[1]; cur544[2] = &b[2]; }
        else { cur5a0[0] = &c[0]; cur5a0[1] = &c[1]; cur544[0] = &a[0]; cur544[1] = &a[1]; cur544[2] = &a[2]; }
        if (!update) return;
        ++memFrames;
        sm.Create(); sm.Create();
        double time = timer + offset;
        size_t i;
        if (lod) for (i = 0; i < maps.size(); ++i) maps[i]->LOD(time);
        if (tess) for (i = 0; i < maps.size(); ++i) maps[i]->Tess();
        for (i = 0; i < maps.size(); ++i) maps[i]->Fore();
        Log(999);
        sm.Restore(); sm.Restore();
    }
};
int main()
{
    int bad = 0;
    StateMgr sm = {}; sm.Mask = 1;
    Map m1 = { 1, 0 }, m2 = { 2, 0 };
    std::vector<Map*> maps; maps.push_back(&m1); maps.push_back(&m2);
    Renderer r = {}; r.Enabled = true; r.mode = 2; r.lod = true; r.tess = false; r.offset = 0.5f;
    r.FrameStart(true, maps, sm, 10.0);
    if (r.cur5a0[0] != &r.c[2] || r.cur544[2] != &r.b[2]) bad |= 1;
    int expect[] = { 101, 102, 301, 302, 999 };
    if (g_n != 5) bad |= 2; else for (int k = 0; k < 5; ++k) if (g_log[k] != expect[k]) bad |= 4;
    if (m1.lastTime != 10.5 || sm.Size != 0 || sm.restores != 2 || sm.Mask != 1 || r.memFrames != 1) bad |= 8;
    g_n = 0; r.mode = 0;
    r.FrameStart(false, maps, sm, 1.0);
    if (r.cur5a0[0] != &r.c[0] || r.cur544[0] != &r.a[0] || g_n != 0 || r.memFrames != 1) bad |= 16;
    r.Enabled = false; r.mode = 2;
    r.FrameStart(true, maps, sm, 1.0);
    if (r.cur5a0[0] != &r.c[0]) bad |= 32;
    if (bad) { std::printf("00b68610_TEST FAIL\n"); return 1; }
    std::printf("00b68610_TEST PASS\n");
    return 0;
}