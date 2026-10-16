// Behaviour test for CTCTurncoat::CTCTurncoat (0x007C78C0): state cleared, override on, timer 1, recovery delay in frames
// (truncated secs * fps) and recovery per frame (perSec / fps).
#include <cstdio>
static long g_fps = 30;
struct Def { float DelaySecs; float PerSec; };
struct Turncoat {
    long WillTurncoatTimer; float Current; long TimeLast; bool Override; long DelayFrames; long Timer; float PerFrame; float Resistance;
    Turncoat(const Def& d) : Current(0), TimeLast(0), Override(true), Timer(0), Resistance(0)
    {
        WillTurncoatTimer = 1;
        DelayFrames = (long)(g_fps * d.DelaySecs);
        PerFrame = d.PerSec / g_fps;
    }
};
int main()
{
    Def d = { 2.5f, 3.0f };
    Turncoat t(d);
    if (t.WillTurncoatTimer != 1 || t.DelayFrames != 75 || t.PerFrame != 0.1f || !t.Override || t.Current != 0 || t.Timer != 0) {
        std::printf("007c78c0_TEST FAIL\n"); return 1;
    }
    std::printf("007c78c0_TEST PASS\n");
    return 0;
}
