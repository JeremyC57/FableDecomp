// CCreatureAction_DragonFireBlastLeft::CCreatureAction_DragonFireBlastLeft(CThingCreatureBase& creature, CThing& target) -- the base constructor with both arguments, then this class's vtable
// Leaf: ??0CCreatureAction_DragonFireBlastLeft@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00953780, 32 bytes (+ int3 padding), thiscall, `ret 8`. Vtable 0x0129422C.
// Identity (proven): CCreatureAction_DragonFireBlastLeft::CCreatureAction_DragonFireBlastLeft(CThingCreatureBase& creature, CThing& target); evidence: the stored vtable 0x0129422C's RTTI is CCreatureAction_DragonFireBlastLeft (<- CCreatureAction_DragonFireBlastBase <- CCombatActionBase <- CCreatureActionBase); the base call is CCreatureAction_DragonFireBlastBase(CThingCreatureBase&, CThing&) 0x00953630; built by CAIStateGroup_JackDragon::UpdateHover 0x00951FB0; FableWin.pdb ??0CCreatureAction_DragonFireBlastLeft@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z.
// Label: the manifest's "CDrawAutoPickups" is propagated (family J100: five byte-identical two-argument forwarding constructors;
//   retail has no identical-code folding).
// Callees: CCreatureAction_DragonFireBlastBase::CCreatureAction_DragonFireBlastBase(CThingCreatureBase&, CThing&) 0x00953630.
// Layout: the base 0x00953630 builds the CCombatActionBase part, stores its own vtable 0x01293F9C and constructs its CIntelligentPointer PShot (+0x114). Local polymorphic partial views (the engine headers of this chain, where they exist, are Ego_r layouts
//   with the vptr as a data word); the class adds no members.
class CThing;
class CThingCreatureBase;
class CCreatureActionBase {                               // partial view: polymorphic root (RTTI CCreatureActionBase <- CBaseObject <- CBaseClass)
public:
    virtual ~CCreatureActionBase();
};
class CCombatActionBase : public CCreatureActionBase {     // partial view
public:
    virtual ~CCombatActionBase();
};
class CCreatureAction_DragonFireBlastBase : public CCombatActionBase {   // partial view (Ego_r.pdb PShot, Line1 ...)
public:
    CCreatureAction_DragonFireBlastBase(CThingCreatureBase& creature, CThing& target);   // 0x00953630
    virtual ~CCreatureAction_DragonFireBlastBase();
};
class CCreatureAction_DragonFireBlastLeft : public CCreatureAction_DragonFireBlastBase {
public:
    CCreatureAction_DragonFireBlastLeft(CThingCreatureBase& creature, CThing& target);
    virtual ~CCreatureAction_DragonFireBlastLeft();
};

CCreatureAction_DragonFireBlastLeft::CCreatureAction_DragonFireBlastLeft(CThingCreatureBase& creature, CThing& target)
    : CCreatureAction_DragonFireBlastBase(creature, target)
{
}