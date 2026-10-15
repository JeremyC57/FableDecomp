// Behaviour test for 0x00852090 (CScriptModuleTransitionBetweenPoints::IsFinished): finished only when
// self-termination is allowed and enabled and the frame is past EndFrame.
#include <cstdio>

static long g_frame = 0;
long GFGetCurrentFrame() { return g_frame; }

struct CScriptModuleTransitionBetweenPoints {
    bool SelfTerminate;
    long EndFrame;
    bool PreventSelfTermination;
    bool IsFinished() const;
};

bool CScriptModuleTransitionBetweenPoints::IsFinished() const
{
    if (!PreventSelfTermination && SelfTerminate && GFGetCurrentFrame() > EndFrame)
        return true;
    return false;
}

int main()
{
    CScriptModuleTransitionBetweenPoints m;
    m.SelfTerminate = true; m.EndFrame = 100; m.PreventSelfTermination = false;
    g_frame = 101;
    if (!m.IsFinished()) { std::printf("00852090_TEST FAIL\n"); return 1; }
    g_frame = 100;
    if (m.IsFinished()) { std::printf("00852090_TEST FAIL\n"); return 1; }
    g_frame = 200; m.PreventSelfTermination = true;
    if (m.IsFinished()) { std::printf("00852090_TEST FAIL\n"); return 1; }
    m.PreventSelfTermination = false; m.SelfTerminate = false;
    if (m.IsFinished()) { std::printf("00852090_TEST FAIL\n"); return 1; }
    std::printf("00852090_TEST PASS\n");
    return 0;
}