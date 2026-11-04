// Behaviour test for CAmbientEmitter::Update 0x00C44E80: the volume is pushed first, Finish runs only when the definition reports
// finished, and the due events are processed last on every update.
#include <cstdio>
static char g_log[16]; static int g_n = 0;
struct CAmbientSourceDef { float time, length; bool IsFinished() const { return time >= length; } };
struct CAmbientEmitter {
    void* m_pOwner; CAmbientSourceDef* m_pDef;
    void UpdateVolume() { g_log[g_n++] = 'V'; }
    void Finish() { g_log[g_n++] = 'F'; }
    void ProcessEvents() { g_log[g_n++] = 'E'; }
    void Update() { UpdateVolume(); if (m_pDef->IsFinished()) Finish(); ProcessEvents(); }
};
int main()
{
    CAmbientSourceDef def = { 1.0f, 5.0f };
    CAmbientEmitter e = { 0, &def };
    e.Update();
    def.time = 5.0f;
    e.Update();
    g_log[g_n] = 0;
    if (g_n != 5 || g_log[0] != 'V' || g_log[1] != 'E' || g_log[2] != 'V' || g_log[3] != 'F' || g_log[4] != 'E') { std::printf("00c44e80_TEST FAIL\n"); return 1; }
    std::printf("00c44e80_TEST PASS\n");
    return 0;
}
