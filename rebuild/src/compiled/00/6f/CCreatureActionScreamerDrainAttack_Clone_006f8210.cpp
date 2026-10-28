// CCreatureAction_ScreamerDrainAttack::Clone() -- Allocate a copy of this creature action.
// Leaf: ?Clone@CCreatureAction_ScreamerDrainAttack@@UBEPAVCCreatureActionBase@@XZ
// Layout: retail CCombatActionBase copy0062f3b0 covers0x114 bytes; fields begin+0x114. Generated headers flatten the vptr and omit the real polymorphic base chain. RTTI proves both base classes; local polymorphic model is required.
#define CCreatureAction_ScreamerDrainAttack CCreatureAction_ScreamerDrainAttack_HeaderLayout
#include "engine/CCreatureAction_ScreamerDrainAttack.h"
#undef CCreatureAction_ScreamerDrainAttack
class CCreatureActionBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10();
    virtual CCreatureActionBase* Clone() const = 0;
    unsigned char _state[0xa0-sizeof(void*)];
};
class CCombatActionBase : public CCreatureActionBase {
public:
    unsigned char _combat_state[0x114-0xa0];
    CCombatActionBase();
    CCombatActionBase(const CCombatActionBase&);
};
class CCreatureAction_CastWillDrainLifeSpell : public CCombatActionBase {
public:
    long Level; long DrainDefIndex;
    virtual CCreatureActionBase* Clone() const;
};
class CCreatureAction_ScreamerDrainAttack : public CCreatureAction_CastWillDrainLifeSpell {
public:
    virtual CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ScreamerDrainAttack::Clone() const { return new CCreatureAction_ScreamerDrainAttack(*this); }