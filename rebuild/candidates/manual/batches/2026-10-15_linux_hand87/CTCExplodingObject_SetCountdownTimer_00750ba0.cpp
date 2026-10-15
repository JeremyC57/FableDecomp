// CTCExplodingObject::SetCountdownTimer(long frames) -- force the thing to update every frame, then start the countdown at `frames`, or shorten an already running countdown to min(frames, CountdownTimer)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00750BA0, length 52 bytes.
// Callees: CThing::SetForceFrameUpdate(bool) @ 0x004C7C60.
// Layout: PDB CTCExplodingObject (48 bytes) over CTCBase (engine/CTCBase.h): CountdownTimer +0x18.
// Shape: the min is the const-reference template (`b < a ? b : a` with a = frames, b = CountdownTimer): retail
// selects the ADDRESS of the member (`lea eax,[esi+18h] ; mov ecx,eax`) or of the argument slot (`lea ecx,[esp+4]`)
// and dereferences it (`mov edx,[ecx] ; mov [eax],edx`); `jle` on the member is the signed `> 0` running test.
#include "engine/CTCBase.h"

struct CThing {
    void SetForceFrameUpdate(bool update);         // 0x004C7C60
};

template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }

struct CTCExplodingObject : CTCBase {
    float MaxDamage;                               // +0x0c
    float Radius;                                  // +0x10
    long  FireDamage;                              // +0x14
    long  CountdownTimer;                          // +0x18
    bool  TriggeredOnCreatureProximity;            // +0x1c
    float TriggerRadius;                           // +0x20
    void SetCountdownTimer(long frames);
};

void CTCExplodingObject::SetCountdownTimer(long frames)
{
    ParentThing->SetForceFrameUpdate(true);
    if (CountdownTimer > 0)
        CountdownTimer = Min(frames, CountdownTimer);
    else
        CountdownTimer = frames;
}
