// CCombatAction_RecoverFromStunned::CCombatAction_RecoverFromStunned(CThingCreatureBase& creature, bool flip) -- the base constructor with both arguments, then this class's vtable
// Leaf: ??0CCombatAction_RecoverFromStunned@@QAE@AAVCThingCreatureBase@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AE1B0, 32 bytes (+ int3 padding), thiscall, `ret 8`. Vtable 0x0127851C.
// Identity (proven): CCombatAction_RecoverFromStunned::CCombatAction_RecoverFromStunned(CThingCreatureBase& creature, bool flip); evidence: the stored vtable 0x0127851C's RTTI is CCombatAction_RecoverFromStunned (<- CCombatAction_GenericStrikeResponseKnockdownRecoverBase <- CCombatActionBase <- CCreatureActionBase); the base call is ...RecoverBase(CThingCreatureBase&, bool) 0x00867AF0; built by CGameScriptInterface::SetThingAsConscious 0x008A9610; FableWin.pdb ??0CCombatAction_RecoverFromStunned@@QAE@AAVCThingCreatureBase@@_N@Z.
// Label: the manifest's "CDrawAutoPickups" is propagated (family J100: five byte-identical two-argument forwarding constructors;
//   retail has no identical-code folding).
// Callees: CCombatAction_GenericStrikeResponseKnockdownRecoverBase::CCombatAction_GenericStrikeResponseKnockdownRecoverBase(CThingCreatureBase&, bool) 0x00867AF0.
// Layout: the base 0x00867AF0 builds the CCombatActionBase part, stores its own vtable 0x01271BAC, clears CanBeHit (+0x114) and stores the bool as Flip (+0x115; Ego_r.pdb CanBeHit +0x12C / Flip +0x12D, retail's CCombatActionBase is 0x18 smaller). Local polymorphic partial views (the engine headers of this chain, where they exist, are Ego_r layouts
//   with the vptr as a data word); the class adds no members.
class CThingCreatureBase;
class CCreatureActionBase {                               // partial view: polymorphic root (RTTI CCreatureActionBase <- CBaseObject <- CBaseClass)
public:
    virtual ~CCreatureActionBase();
};
class CCombatActionBase : public CCreatureActionBase {     // partial view
public:
    virtual ~CCombatActionBase();
};
class CCombatAction_GenericStrikeResponseKnockdownRecoverBase : public CCombatActionBase {   // partial view (retail CanBeHit +0x114, Flip +0x115)
public:
    CCombatAction_GenericStrikeResponseKnockdownRecoverBase(CThingCreatureBase& creature, bool flip);   // 0x00867AF0
    virtual ~CCombatAction_GenericStrikeResponseKnockdownRecoverBase();
};
class CCombatAction_RecoverFromStunned : public CCombatAction_GenericStrikeResponseKnockdownRecoverBase {
public:
    CCombatAction_RecoverFromStunned(CThingCreatureBase& creature, bool flip);
    virtual ~CCombatAction_RecoverFromStunned();
};

CCombatAction_RecoverFromStunned::CCombatAction_RecoverFromStunned(CThingCreatureBase& creature, bool flip)
    : CCombatAction_GenericStrikeResponseKnockdownRecoverBase(creature, flip)
{
}