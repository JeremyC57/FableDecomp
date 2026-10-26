// Behaviour test for 0x00820AB0 CMap::UpdateScriptedMapBrushes: expired entries are erased (order kept), live ones applied with their progress.
#include <cstdio>
struct Brush { float CreationTimeSeconds; };
struct Area { float TimeLeft; int name; };
static int g_fps = 4;
static Brush g_brush = { 2.0f };
static float g_progress[8]; static int g_applied = 0;
class CMap { public:
    Area areas[8]; Area* start; Area* finish;
    Brush* GetScriptedMapBrush(int) { return &g_brush; }
    void Apply(float p, Brush*) { g_progress[g_applied++] = p; }
    Area* erase(Area* pos)
    {
        if (pos + 1 != finish) for (Area* s = pos + 1, *d = pos; s != finish; ++s, ++d) *d = *s;
        --finish;
        return pos;
    }
    void Update()
    {
        if (start != finish) {
            for (Area* it = start; it != finish; ) {
                Brush* brush = GetScriptedMapBrush(it->name);
                it->TimeLeft -= 1.0f / (float)g_fps;
                if (it->TimeLeft < 0.0f) it = erase(it);
                else { Apply((brush->CreationTimeSeconds - it->TimeLeft) / brush->CreationTimeSeconds, brush); ++it; }
            }
        }
    }
};
int main()
{
    CMap m; m.start = m.areas; m.finish = m.areas + 3;
    m.areas[0].TimeLeft = 1.0f; m.areas[0].name = 1;
    m.areas[1].TimeLeft = 0.125f; m.areas[1].name = 2;      // expires (0.125 - 0.25 < 0)
    m.areas[2].TimeLeft = 2.25f; m.areas[2].name = 3;
    m.Update();
    bool ok = m.finish - m.start == 2 && m.areas[0].name == 1 && m.areas[1].name == 3 && g_applied == 2 &&
              g_progress[0] == 0.625f && g_progress[1] == 0.0f && m.areas[1].TimeLeft == 2.0f;
    if (!ok) { std::printf("00820ab0_TEST FAIL\n"); return 1; }
    std::printf("00820ab0_TEST PASS\n");
    return 0;
}