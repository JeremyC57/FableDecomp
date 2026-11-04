// Behaviour test for the sample playback Initialize (0x00C351B0): a model of the row -- lengths from the loop points, the start
// position from the fraction, and the section it falls in (0 intro, 1 loop, 2 tail which drops the loop handle, 3 past the end).
#include <stdio.h>
struct State { long Section; unsigned long Position, Intro, Loop, Tail; void* LoopHandle; float PlayTime; };
static void Initialize(State& s, unsigned long length, unsigned long loopStart, unsigned long loopEnd, float fraction, void* handle, float duration, float speed)
{
    s.LoopHandle = handle;
    s.Intro = loopStart;
    s.Loop = loopEnd - s.Intro;
    s.Tail = length - s.Loop - s.Intro;
    unsigned long intro = s.Intro, loop = s.Loop, tail = s.Tail;
    unsigned long position = (unsigned long)((float)length * fraction);
    if (position < intro) { s.Section = 0; s.Position = position; }
    else if (position < intro + loop) { s.Section = 1; s.Position = position - intro; }
    else if (position < tail + loop + intro) { s.Position = position - loop - intro; s.Section = 2; if (s.LoopHandle) s.LoopHandle = 0; }
    else s.Section = 3;
    s.PlayTime = duration * speed;
}
int main()
{
    State s; int h;
    Initialize(s, 1000, 100, 600, 0.05f, &h, 2.0f, 1.5f);
    int ok = s.Section == 0 && s.Position == 50 && s.Intro == 100 && s.Loop == 500 && s.Tail == 400 && s.PlayTime == 3.0f && s.LoopHandle == &h;
    Initialize(s, 1000, 100, 600, 0.3f, &h, 2.0f, 1.0f); ok = ok && s.Section == 1 && s.Position == 200;
    Initialize(s, 1000, 100, 600, 0.75f, &h, 2.0f, 1.0f); ok = ok && s.Section == 2 && s.Position == 150 && s.LoopHandle == 0;
    printf(ok ? "00c351b0_TEST PASS\n" : "00c351b0_TEST FAIL\n");
    return ok ? 0 : 1;
}