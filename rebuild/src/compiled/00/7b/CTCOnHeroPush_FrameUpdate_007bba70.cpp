// CTCOnHeroPush::FrameUpdate() -- counts frames since the hero pushed; after five seconds' worth (five times the constant frame rate) the component removes itself
// Leaf: ?FrameUpdate@CTCOnHeroPush@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BBA70, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 9 of CTCOnHeroPush's vtable 0x0123CBC4 (the slot holds this body in 1 vtable(s), owner CTCOnHeroPush); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts FrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?FrameUpdate@CTCOnHeroPush@@UAEXXZ; `ret` matches the signature.
// Was: CTCOnHeroPush::vfunc_9 (rebuild/src/compiled/00/7b/CTCOnHeroPush_vfunc9_007bba70.cpp).
// No shared header for CTCOnHeroPush: minimal local definition on its RTTI base CTCBase (Ego_r.pdb members DistPushed +0x0C,
// PushDir +0x10, FrameSincePushed +0x1C). The frame limit is a function-local static (guard 0x013BAE78 bit 0, value 0x013BAE74);
// its name s_MaxFrames is descriptive (no data-symbol name source).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/C3DVector.h"
#include "engine/CMainGameComponent.h"
class CTCOnHeroPush : public CTCBase {
public:
    virtual void FrameUpdate();                              // slot 9
    float DistPushed;                                        // +0x0C
    C3DVector PushDir;                                       // +0x10
    long FrameSincePushed;                                   // +0x1C
};
void CTCOnHeroPush::FrameUpdate()
{
    static long s_MaxFrames = CMainGameComponent::GetConstantFPS() * 5;
    if (++FrameSincePushed > s_MaxFrames)
        ParentThing->RemoveTC(TCI_ON_HERO_PUSH);
}
