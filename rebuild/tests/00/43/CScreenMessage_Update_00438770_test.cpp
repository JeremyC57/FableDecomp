// Behaviour test for 0x00438770: CScreenMessage::Update: frames - 1, characters += the GUI def's per-frame count (def loaded once), clamped to [0, length]
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct Def { long perFrame; };
static Def g_defStore = { 3 };
static Def* g_def = 0; static int g_loads = 0;
static const Def* GetDef() { if (!g_def) { ++g_loads; g_def = &g_defStore; } return g_def; }
struct Msg {
    long length; long frames, chars;
    void Update()
    {
        --frames;
        chars += GetDef()->perFrame;
        long len = length;
        if (chars < 0) chars = 0; else if (chars > len) chars = len;
    }
};
static void Run()
{
    Msg m; m.length = 7; m.frames = 5; m.chars = 0;
    m.Update(); CHECK(m.frames == 4 && m.chars == 3 && g_loads == 1);
    m.Update(); m.Update(); CHECK(m.chars == 7 && m.frames == 2 && g_loads == 1);
    g_defStore.perFrame = -20; m.Update(); CHECK(m.chars == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00438770_TEST FAIL\n"); return 1; }
    std::printf("00438770_TEST PASS\n");
    return 0;
}