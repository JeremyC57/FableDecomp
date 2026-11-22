#ifndef FABLE_ENGINE_CCREATUREACTION_DRAGONFIREBEAMINTO_H
#define FABLE_ENGINE_CCREATUREACTION_DRAGONFIREBEAMINTO_H
#ifdef FABLE_RETAIL_JACKDRAGON_HOVER_PASS27
#include "engine/CCombatActionBase.h"
// FableWin 02BE2090 and Ego_r put Timer after the combat-action base.
class CCreatureAction_DragonFireBeamInto : public CCombatActionBase {
public:
    CCreatureAction_DragonFireBeamInto(CThingCreatureBase& creature,CThing& target,float time);
    virtual CCreatureActionBase* Clone() const;
#ifdef FABLE_RETAIL_CCREATUREACTION_DRAGONFIREBEAMINTO_INITIALISE
    // NF255 opt-in (2026-11-22, q12720, row 0x009535B0 and its Clone / deleting-destructor rows): the class's own override of
    // CCombatActionBase::Initialise, Ego_r.pdb 0x00919B50 + FableWin.pdb 0x02AFC3E0 ?Initialise@CCreatureAction_DragonFireBeamInto@@UAEXXZ
    // (public virtual), retail vftable 0x01293E54 slot 12 = 0x008F41D0 (landed, same Leaf). Declaration only; no layout change.
    virtual void Initialise();                                           // 0x008F41D0
#endif
private:
    float Timer;
};
FABLE_STATIC_ASSERT_AT(13, sizeof(CCreatureAction_DragonFireBeamInto)==0x118);
#endif
#endif
