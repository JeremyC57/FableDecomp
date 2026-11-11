// NCreatureMode::CModeRockTrollVulnerable::OnDeactivate() -- leaving the mode: marks it inactive
// Leaf: ?OnDeactivate@CModeRockTrollVulnerable@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6840, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 6 of NCreatureMode::CModeRockTrollVulnerable's vtable 0x01285774 (the slot holds this body in 1 vtable(s), owner NCreatureMode::CModeRockTrollVulnerable); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts OnDeactivate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnDeactivate@CModeRockTrollVulnerable@NCreatureMode@@UAEXXZ; `ret` matches the signature.
// Was: NCreatureMode::CModeRockTrollVulnerable::vfunc_6 (rebuild/src/compiled/00/8d/NCreatureModeCModeRockTrollVulnerable_vfunc6_008d6840.cpp).
// No shared header for NCreatureMode::CModeRockTrollVulnerable: minimal local definition (RTTI base CTCCreatureModeManager::CModeBase), only the
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
class CModeRockTrollVulnerable : public CTCCreatureModeManager::CModeBase {
public:
    CModeRockTrollVulnerable(CThing& thing);
    virtual void OnDeactivate();
};
void CModeRockTrollVulnerable::OnDeactivate()
{
    Active = false;
}
}
