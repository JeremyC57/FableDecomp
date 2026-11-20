#ifndef FABLE_ENGINE_NANIMATIONENTRYBODGE_CANIMENTRYLITE_H
#define FABLE_ENGINE_NANIMATIONENTRYBODGE_CANIMENTRYLITE_H
#include "rebuild_abi.h"
class CAnimationEntry;class CThing;
namespace NAnimationEntryBodge {
// Layout: complete original Ego28550/FW2ab5d,16 bytes; native BankIndex read at0.
class CAnimEntryLite {public:
 CAnimEntryLite(const CAnimationEntry&);
 long BankIndex,DelayTime,TransitionTime,AnimFlags;
#ifdef FABLE_RETAIL_CANIMENTRYLITE_INLINE_ACCESSORS
 // NF192 opt-in (2026-11-20, q6370, row 0x00861A80 CModeStandBase::SetUpMovementAnim): the two accessors with their bodies (FableWin.pdb
 // ?GetDelayTime@CAnimEntryLite@NAnimationEntryBodge@@QBEJXZ / ?GetAnimateThingFlags@...@@QBEJXZ; Ego_r.pdb keeps no copy: inline in
 // release, read straight from DelayTime +0x04 / AnimFlags +0x0C in 0x00861A80). Default path unchanged.
 long GetDelayTime()const{return DelayTime;}
 long GetAnimateThingFlags()const{return AnimFlags;}
#else
 long GetDelayTime()const;
 long GetAnimateThingFlags()const;
#endif
 long GetTimeToBlendIn(const CThing&)const;
};
}
FABLE_STATIC_ASSERT_AT(15, sizeof(NAnimationEntryBodge::CAnimEntryLite)==16);
#endif
