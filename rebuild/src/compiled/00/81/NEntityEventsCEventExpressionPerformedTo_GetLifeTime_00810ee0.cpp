// NEntityEvents::CEventExpressionPerformedTo::GetLifeTime() const -- the event lives ten seconds: ten times the constant frame rate (CMainGameComponent::ConstantFPS) in world frames
// Leaf: ?GetLifeTime@CEventExpressionPerformedTo@NEntityEvents@@UBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00810EE0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 3 of NEntityEvents::CEventExpressionPerformedTo's vtable 0x01274240 (the slot holds this body in 1 vtable(s), owner NEntityEvents::CEventExpressionPerformedTo); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetLifeTime(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetLifeTime@CEventExpressionPerformedTo@NEntityEvents@@UBEJXZ; `ret` matches the signature.
// Was: NEntityEvents::CEventExpressionPerformedTo::vfunc_3 (rebuild/src/compiled/00/81/NEntityEventsCEventExpressionPerformedTo_vfunc3_00810ee0.cpp).
// No shared header for NEntityEvents::CEventExpressionPerformedTo: minimal local definition on its RTTI base NEntityEvents::CEventBase.
#define FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS3
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/NEntityEvents_CEventHitBase.h"
#include "engine/CMainGameComponent.h"
namespace NEntityEvents {
class CEventExpressionPerformedTo : public CEventBase {
public:
    virtual long GetLifeTime() const;                    // slot 3
};
long CEventExpressionPerformedTo::GetLifeTime() const
{
    return CMainGameComponent::GetConstantFPS() * 10;
}
}
