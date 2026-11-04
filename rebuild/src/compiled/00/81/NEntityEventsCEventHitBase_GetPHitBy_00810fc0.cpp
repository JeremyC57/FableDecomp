// NEntityEvents::CEventHitBase::GetPHitBy() const -- the thing PThingHitting names in this hit event's hit parameters
// Leaf: ?GetPHitBy@CEventHitBase@NEntityEvents@@QBEPAVCThing@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00810FC0, 11 bytes: `mov ecx,[ecx+8] ; add ecx,50h ; jmp 0x00A01B50`
//   (PHitParams->GetPThingHitting(): CIntelligentPointer<CThing>::Get on CHitParameters::PThingHitting, tail call to GetPItem).
// Identity (two sources): Ego_r.pdb ?GetPHitBy@CEventHitBase@NEntityEvents@@QBEPAVCThing@@XZ, in donor order right after the
//   CIsHitEventHasHitByThing functor (retail 0x00810F90) and with GetPHitBy / GetPWeaponUsed adjacent (retail 0x00810FC0 / 0x00810FD0);
//   the body reads CHitParameters::PThingHitting at 0x50 (engine/CHitParameters.h: Ego_r +0x58 less 8 for the two STLport
//   containers), and PHitParams is the Ego_r CEventHitBase member at +8. Callers: CGameScriptThing::Msg* hit queries and the hit-event filters.
// Callees: ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ 0x00A01B50 (through CIntelligentPointer<CThing>::Get()).
// Header: engine/NEntityEvents_CEventHitBase.h (new), engine/CHitParameters.h, engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER),
//   engine/CThing.h (FABLE_RETAIL_CTHING: CThing is a CBaseObject, so Get()'s cast is a no-op).
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/NEntityEvents_CEventHitBase.h"
namespace NEntityEvents {
CThing* CEventHitBase::GetPHitBy() const
{
    return PHitParams->GetPThingHitting();
}
}