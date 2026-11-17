#ifndef FABLE_ENGINE_NENTITYEVENTS_CEVENTFIREDSHOT_H
#define FABLE_ENGINE_NENTITYEVENTS_CEVENTFIREDSHOT_H
// New header (e06 egod lane, row 0x008BF280 CCreatureAction_GenericFireProjectileWeapon::PerformAction): NEntityEvents::CEventFiredShot,
// 16 bytes (`push 10h ; call operator new`, 0x008BF4BD): CEventBase (vptr, WorldFrame +4) then CIntelligentPointer<CThing> PTarget +8
// (Ego_r.pdb record; landed ctor 0x008BFE20). RTTI vtable 0x0127F2C0 .?AVCEventFiredShot@NEntityEvents@@. FableWin.pdb
// ??0CEventFiredShot@NEntityEvents@@QAE@AAVCThing@@@Z (out of line at 0x008BFE20), ?GetType 0x008BFE60, ?GetLifeTime 0x008BFE70.
// CEventBase from engine/NEntityEvents_CEventHitBase.h. Declarations only. Needs FABLE_RETAIL_CINTELLIGENTPOINTER.
#include "engine/NEntityEvents_CEventHitBase.h"
#include "engine/CIntelligentPointer.h"
class CThing;
namespace NEntityEvents {
class CEventFiredShot : public CEventBase {
public:
    CEventFiredShot(CThing& target);                                     // 0x008BFE20
private:
    CIntelligentPointer<CThing> PTarget;                                 // +0x08
};
}
#endif
