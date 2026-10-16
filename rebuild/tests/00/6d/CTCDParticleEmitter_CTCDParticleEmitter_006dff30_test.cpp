#include <cstdio>
// Behaviour model of CTCDParticleEmitter::CTCDParticleEmitter (0x006DFF30).
struct Emitter {
    long ParticleTypeID; unsigned long AttachmentFlags; long AttachmentPosIndex; float FloatOffset; long DieOffTimer; long FramesLeft;
    void* Data; void* Info;
    bool Waiting : 1; bool Force : 1; bool Independant : 1; bool Disabled : 1; bool ResetPos : 1; bool ResetOri : 1; bool Camera : 1; bool Moving : 1;
    bool Attached : 1; bool AlwaysUpdate : 1; bool Asleep : 1;
    Emitter() : ParticleTypeID(0), AttachmentFlags(0), AttachmentPosIndex(0), FloatOffset(0), DieOffTimer(-1), FramesLeft(0), Data(0), Info(0)
    { Waiting = false; Force = false; Independant = false; Disabled = false; ResetPos = true; ResetOri = true; Camera = false; Moving = false;
      Attached = false; AlwaysUpdate = false; Asleep = false; }
};
int main()
{
    Emitter e;
    if (e.DieOffTimer != -1 || e.FramesLeft || e.Data || !e.ResetPos || !e.ResetOri || e.Disabled || e.Asleep || e.Attached) { std::printf("006dff30_TEST FAIL\n"); return 1; }
    std::printf("006dff30_TEST PASS\n");
    return 0;
}