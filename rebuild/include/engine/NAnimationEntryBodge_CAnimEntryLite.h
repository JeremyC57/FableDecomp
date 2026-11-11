#ifndef FABLE_ENGINE_NANIMATIONENTRYBODGE_CANIMENTRYLITE_H
#define FABLE_ENGINE_NANIMATIONENTRYBODGE_CANIMENTRYLITE_H
#include "rebuild_abi.h"
class CAnimationEntry;class CThing;
namespace NAnimationEntryBodge {
// Layout: complete original Ego28550/FW2ab5d,16 bytes; native BankIndex read at0.
class CAnimEntryLite {public:
 CAnimEntryLite(const CAnimationEntry&);
 long BankIndex,DelayTime,TransitionTime,AnimFlags;
 long GetDelayTime()const;
 long GetAnimateThingFlags()const;
 long GetTimeToBlendIn(const CThing&)const;
};
}
FABLE_STATIC_ASSERT(sizeof(NAnimationEntryBodge::CAnimEntryLite)==16);
#endif
