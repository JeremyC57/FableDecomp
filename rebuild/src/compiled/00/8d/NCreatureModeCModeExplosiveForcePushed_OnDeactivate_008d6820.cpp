// NCreatureMode::CModeExplosiveForcePushed::OnDeactivate() -- leaving the mode: marks it inactive
// Leaf: ?OnDeactivate@CModeExplosiveForcePushed@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6820, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 6 of NCreatureMode::CModeExplosiveForcePushed's vtable 0x012770B4 (the slot holds this body in 1 vtable(s), owner NCreatureMode::CModeExplosiveForcePushed); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts OnDeactivate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnDeactivate@CModeExplosiveForcePushed@NCreatureMode@@UAEXXZ; `ret` matches the signature.
// Was: NCreatureMode::CModeExplosiveForcePushed::vfunc_6 (rebuild/src/compiled/00/8d/NCreatureModeCModeExplosiveForcePushed_vfunc6_008d6820.cpp).
// No shared header for NCreatureMode::CModeExplosiveForcePushed: minimal local definition (RTTI base CTCCreatureModeManager::CModeBase), only the
// override. The body is `mov byte ptr [ecx+18h],0`: CModeBase::Active (+0x18).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
namespace NCreatureMode {
class CModeExplosiveForcePushed : public CTCCreatureModeManager::CModeBase {
public:
    CModeExplosiveForcePushed(CThing& thing);
    virtual void OnDeactivate();
};
void CModeExplosiveForcePushed::OnDeactivate()
{
    Active = false;
}
}
