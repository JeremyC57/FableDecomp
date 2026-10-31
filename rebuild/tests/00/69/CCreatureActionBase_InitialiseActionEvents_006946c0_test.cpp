// Behaviour test for CCreatureActionBase::InitialiseActionEvents (0x006946C0): every embedded animation event whose first
// word names a registered event is allocated at delay + fps * time and kept only when it accepts the full name.
#include <cstdio>
#include <cstring>

struct CEvent { long Frame; const char* Name; bool Accepts(const char* full) const { return std::strchr(full, '!') == 0; } };
struct CRegistered { const char* Hook; };
static CRegistered g_registered[2] = { { "SOUND" }, { "SHAKE" } };
static bool g_editor;
static const char* g_names[4] = { "SOUND one", "SHAKE", "OTHER x", "SOUND bad!" };
static float g_times[4] = { 0.5f, 1.0f, 2.0f, 0.25f };

static const CRegistered* Find(const char* hook, long length)
{
    for (int i = 0; i < 2; ++i)
        if ((long)std::strlen(g_registered[i].Hook) == length && std::strncmp(g_registered[i].Hook, hook, length) == 0)
            return &g_registered[i];
    return 0;
}

struct CAction {
    long AnimDelay; long Animation; long FramesPerSecond;
    CEvent Events[8]; int Count;
    void InitialiseActionEvents()
    {
        if (g_editor) return;
        long animation = Animation;
        if (animation <= 0) return;
        long fps = FramesPerSecond;
        for (int i = 0; i < 4; ++i) {
            const char* name = g_names[i];
            const char* space = std::strchr(name, ' ');
            long end = space ? (long)(space - name) : (long)std::strlen(name);
            const CRegistered* node = Find(name, end);
            if (node) {
                long frame = (long)(AnimDelay + fps * g_times[i]);
                CEvent e; e.Frame = frame; e.Name = node->Hook;
                if (e.Accepts(name)) Events[Count++] = e;
            }
        }
    }
};

int main()
{
    CAction a; a.AnimDelay = 3; a.Animation = 7; a.FramesPerSecond = 10; a.Count = 0;
    a.InitialiseActionEvents();
    if (a.Count != 2 || a.Events[0].Frame != 8 || a.Events[1].Frame != 13 || std::strcmp(a.Events[1].Name, "SHAKE") != 0) { std::printf("006946c0_TEST FAIL\n"); return 1; }
    CAction b = a; b.Count = 0; b.Animation = 0; b.InitialiseActionEvents();
    if (b.Count != 0) { std::printf("006946c0_TEST FAIL\n"); return 1; }
    g_editor = true; a.Count = 0; a.InitialiseActionEvents();
    if (a.Count != 0) { std::printf("006946c0_TEST FAIL\n"); return 1; }
    std::printf("006946c0_TEST PASS\n");
    return 0;
}