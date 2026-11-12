// CTCMultiArrow::FrameUpdate() -- while ammo effects remain, counts down to the next CreateAmmoEffect (unless a cut scene cancelled it); once out of shots, removes this component (TCI_MULTI_ARROW)
// Leaf: ?FrameUpdate@CTCMultiArrow@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CC490, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 9 of CTCMultiArrow's vtable 0x0126ED7C (the slot holds this body in 1 vtable(s), owner CTCMultiArrow); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts FrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?FrameUpdate@CTCMultiArrow@@UAEXXZ; `ret` matches the signature.
// Was: CTCMultiArrow::vfunc_9 (rebuild/src/compiled/00/7c/CTCMultiArrow_vfunc9_007cc490.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCMULTIARROW_VSLOTS3
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CTCMultiArrow.h"
void CTCMultiArrow::FrameUpdate()
{
    if (CreateAmmoEffectCount > 0) {
        if (!CutSceneCancelled && --CreateAmmoEffectTimer == 0) {
            CreateAmmoEffectTimer = CreateAmmoEffectDelay;
            CreateAmmoEffect();
        }
    } else if (NumberOfShots <= 0) {
        ParentThing->RemoveTC(TCI_MULTI_ARROW);
    }
}
