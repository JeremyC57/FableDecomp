// NEntityEvents::CEventHeroHasBeenSpokenToByVillagers::GetLifeTime() const -- the event lives ten seconds: ten times the constant frame rate (CMainGameComponent::ConstantFPS) in world frames
// Leaf: ?GetLifeTime@CEventHeroHasBeenSpokenToByVillagers@NEntityEvents@@UBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00810EA0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 3 of NEntityEvents::CEventHeroHasBeenSpokenToByVillagers's vtable 0x01276E00 (the slot holds this body in 1 vtable(s), owner NEntityEvents::CEventHeroHasBeenSpokenToByVillagers); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetLifeTime(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetLifeTime@CEventHeroHasBeenSpokenToByVillagers@NEntityEvents@@UBEJXZ; `ret` matches the signature.
// Was: NEntityEvents::CEventHeroHasBeenSpokenToByVillagers::vfunc_3 (rebuild/src/compiled/00/81/NEntityEventsCEventHeroHasBeenSpokenToByVillagers_vfunc3_00810ea0.cpp).
// No shared header for NEntityEvents::CEventHeroHasBeenSpokenToByVillagers: minimal local definition on its RTTI base NEntityEvents::CEventBase.
#define FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS3
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/NEntityEvents_CEventHitBase.h"
#include "engine/CMainGameComponent.h"
namespace NEntityEvents {
class CEventHeroHasBeenSpokenToByVillagers : public CEventBase {
public:
    virtual long GetLifeTime() const;                    // slot 3
};
long CEventHeroHasBeenSpokenToByVillagers::GetLifeTime() const
{
    return CMainGameComponent::GetConstantFPS() * 10;
}
}
