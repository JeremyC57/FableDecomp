// CTCNoise::FrameUpdate() -- counts the noise's life down and kills the noise thing when it runs out
// Leaf: ?FrameUpdate@CTCNoise@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00749860, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 9 of CTCNoise's vtable 0x01268A74 (the slot holds this body in 1 vtable(s), owner CTCNoise); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts FrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?FrameUpdate@CTCNoise@@UAEXXZ; `ret` matches the signature.
// Was: CTCNoise::vfunc_9 (rebuild/src/compiled/00/74/CTCNoise_vfunc9_00749860.cpp).
#define FABLE_RETAIL_HEARING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCNoise.h"
void CTCNoise::FrameUpdate()
{
    if (LifeTimer > 0)
        --LifeTimer;
    if (LifeTimer == 0)
        ParentThing->Kill(true);
}
