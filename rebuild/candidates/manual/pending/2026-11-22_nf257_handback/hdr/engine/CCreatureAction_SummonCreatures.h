#ifndef FABLE_ENGINE_CCREATUREACTION_SUMMONCREATURES_H
#define FABLE_ENGINE_CCREATUREACTION_SUMMONCREATURES_H

#include "engine/CCombatActionBase.h"
class CThingCreatureBase;

// Original class hierarchy and constructor types from Ego_r and FableWin.
class CCreatureAction_SummonCreatures : public CCombatActionBase {
public:
    CCreatureAction_SummonCreatures(CThingCreatureBase& creature, CThingCreatureBase& target, bool spawn_on_target);
#ifdef FABLE_RETAIL_CCREATUREACTION_SUMMONCREATURES_OVERRIDES
    // NF257 opt-in (2026-11-22, q12722, rows 0x008B83C0 / 0x008B83E0 / 0x008B8440 / 0x008B89A0 / 0x008B87A0): the other two constructors
    // and the class's own overrides, Ego_r.pdb 0x008F1140 / 0x008F11C0 / 0x008F1520 / 0x008F0930 + FableWin.pdb 0x029FF160 / 0x029FF3A0 /
    // 0x029FF1A0 / 0x029FF450 (public; Clone and Initialise public virtual), retail 0x008B83C0 / 0x008B8440 and vftable 0x0127ADDC slot 11 =
    // 0x008B89A0, slot 12 = 0x008B7A20 (landed, same Leaf). Declarations only; no layout change.
    CCreatureAction_SummonCreatures();                                   // 0x008B83C0
    CCreatureAction_SummonCreatures(CThingCreatureBase& creature);       // 0x008B8440
    virtual CCreatureActionBase* Clone() const;                          // 0x008B89A0
    virtual void Initialise();                                           // 0x008B7A20
#endif
private:
    bool SpawnOnTarget;
};
FABLE_STATIC_ASSERT_AT(33, sizeof(CCreatureAction_SummonCreatures) == 0x118);

#endif
