// Behaviour test for 0x00717950 NTCScriptedControl::CActionTalkBase::CActionTalkBase (see src/00717950.cpp): the ctor zeroes the
// counters and pointers, sets the tag indices / gesture counters to -1, the conversation attitude to 1, copies the three flags,
// and runs the one-time default camera-tag initialisation on the first construction only.
#include <cstdio>
static int g_inits; static bool g_needInit = true;
struct Talk {
    long UpdateCounter; double Started, Lasts; long CurrentMessageIndex, GameTurnDuration;
    long NextAttitude, NextAnimation, NextCamera, LastPriority, FramesUntilGesture, LastGesture, CameraMoved, Attitude;
    bool MakeTargetListen, UseCameraTags, SoundIn2D, GettingUp;
    Talk(bool listen, bool tags, bool in2D)
        : UpdateCounter(0), Started(0.0), Lasts(0.0), CurrentMessageIndex(0), GameTurnDuration(0), NextAttitude(-1), NextAnimation(-1),
          NextCamera(-1), LastPriority(-1), FramesUntilGesture(-1), LastGesture(-1), CameraMoved(0), Attitude(1),
          MakeTargetListen(listen), UseCameraTags(tags), SoundIn2D(in2D), GettingUp(false)
    {
        if (g_needInit) { g_needInit = false; ++g_inits; }
    }
};
int main()
{
    Talk a(true, false, true), b(false, true, false);
    if (g_inits != 1) { std::printf("00717950_TEST FAIL\n"); return 1; }
    if (a.NextCamera != -1 || a.Attitude != 1 || a.UpdateCounter != 0 || a.Started != 0.0) { std::printf("00717950_TEST FAIL\n"); return 1; }
    if (!a.MakeTargetListen || a.UseCameraTags || !a.SoundIn2D || b.MakeTargetListen || !b.UseCameraTags) { std::printf("00717950_TEST FAIL\n"); return 1; }
    std::printf("00717950_TEST PASS\n");
    return 0;
}