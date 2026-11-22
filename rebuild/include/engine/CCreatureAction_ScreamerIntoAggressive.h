#ifndef FABLE_ENGINE_CCREATUREACTION_SCREAMERINTOAGGRESSIVE_H
#define FABLE_ENGINE_CCREATUREACTION_SCREAMERINTOAGGRESSIVE_H
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"
// 0096DF50: original class is TU-local in both PDBs and has no data of its own.
namespace {
class CCreatureAction_ScreamerIntoAggressive : public CCombatActionBase {
public:
#ifdef FABLE_RETAIL_CCREATUREACTION_SCREAMERINTOAGGRESSIVE_CTOR_OUTOFLINE
    // Opt-in (0x0096DB60, the constructor's own row): declaration only, the row defines it out of line (retail keeps an out-of-line
    // copy at 0x0096DB60; FableWin.pdb S_PUB32 ??0CCreatureAction_ScreamerIntoAggressive@?A0x4c815337@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z).
    CCreatureAction_ScreamerIntoAggressive(CThingCreatureBase& creature,CThing& target);
#else
    CCreatureAction_ScreamerIntoAggressive(CThingCreatureBase& creature,CThing& target)
        : CCombatActionBase(creature,target,150,CSoundPair()) {}
#endif
    virtual void Initialise();
    virtual CCreatureActionBase* Clone() const;
    virtual bool IsToFaceTarget() const { return true; }
    virtual bool IsToFaceTargetAtStart() const { return true; }
};
}
FABLE_STATIC_ASSERT_AT(17, sizeof(CCreatureAction_ScreamerIntoAggressive)==0x114);
#endif
