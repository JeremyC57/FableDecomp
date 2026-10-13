// CTCTargetingPlayer::SetAsTargetNextThingInDirection(ETargetNextThingMethod)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008023C0, oracle length 26 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[esp+4] ; push esi ; mov esi,ecx ; mov byte ptr [esi+3Ch],1 ; mov [esi+40h],eax ; call <frame counter> ; mov [esi+44h],eax ; pop esi ; ret 4
// engine/CTCTargetingPlayer.h (PDB): +0x3C bool TargetNextThing, +0x40 ETargetNextThingMethod
// TargetNextThingMethod, +0x44 long FrameSetAsTargetNextThing. Queues a "target the next thing"
// request and stamps it with the current frame from the global frame counter at 0x0049D870
// (__cdecl, no arguments; its manifest label is a propagated name and is not used here).
#include "engine/CTCTargetingPlayer.h"

typedef long ETargetNextThingMethod;

extern long GFGetCurrentFrame();    // 0x0049D870

struct CTCTargetingPlayer_Methods : CTCTargetingPlayer {
    void SetAsTargetNextThingInDirection(ETargetNextThingMethod method);
};

void CTCTargetingPlayer_Methods::SetAsTargetNextThingInDirection(ETargetNextThingMethod method)
{
    this->TargetNextThing = true;
    *(ETargetNextThingMethod*)&this->TargetNextThingMethod[0] = method;
    this->FrameSetAsTargetNextThing = GFGetCurrentFrame();
}
