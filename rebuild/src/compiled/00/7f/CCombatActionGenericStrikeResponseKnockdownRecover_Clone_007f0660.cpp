// CCombatAction_GenericStrikeResponseKnockdownRecover::Clone() -- Allocate a copy of this creature action.
// Leaf: ?Clone@CCombatAction_GenericStrikeResponseKnockdownRecover@@UBEPAVCCreatureActionBase@@XZ
// Layout: retail CCombatActionBase copy0062f3b0 covers0x114 bytes; fields begin+0x114. Generated headers flatten the vptr and omit the real polymorphic base chain. RTTI proves both base classes; local polymorphic model is required.
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
class CCombatAction_GenericStrikeResponseKnockdownRecoverBase : public CCombatActionBase {
public:
    bool CanBeHit; bool Flip;
    virtual CCreatureActionBase* Clone() const;
};
class CCombatAction_GenericStrikeResponseKnockdownRecover : public CCombatAction_GenericStrikeResponseKnockdownRecoverBase {
public:
    virtual CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCombatAction_GenericStrikeResponseKnockdownRecover::Clone() const { return new CCombatAction_GenericStrikeResponseKnockdownRecover(*this); }