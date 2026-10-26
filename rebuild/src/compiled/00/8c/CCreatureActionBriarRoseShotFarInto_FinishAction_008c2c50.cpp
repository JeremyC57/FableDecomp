// CCreatureAction_BriarRoseShotFarInto::FinishAction() -- base FinishAction, then hand the creature a temporary CCreatureAction_BriarRoseShotFarOutof(creature, target) as its current action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C2C50, length 91 bytes. Vtable slot 4 (FinishAction) of
// CCreatureAction_BriarRoseShotFarInto (0x01294A2C).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8 into edi, PTarget at +0xa8 into ebx: both named
// locals fetched BEFORE the base call), CCombatActionBase::FinishAction @ 0x00859A20 (base virtual, non-virtual call),
// CCreatureAction_BriarRoseShotFarOutof::CCreatureAction_BriarRoseShotFarOutof(CThingCreatureBase*, CThingCreatureBase*)
// @ 0x008C39A0 (the 0x11c-byte temporary at the bottom of the frame), CThingCreatureBase::SetCurrentAction(const
// CCreatureActionBase&) @ 0x006644F0 (`push eax` of the constructor's returned this), then the temporary's inlined
// destructor: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 on its PTarget member (+0x114) and
// CCombatActionBase::~CCombatActionBase @ 0x0062E900 (manifest "~CTCLightningOrb" is propagated).
// Shape: the implicit destructor of the temporary is inlined as member destructor + base destructor with NO vtable
// stores: both the temporary's class and its CIntelligentPointer member are modelled `__declspec(novtable)` (their own
// constructors are out of line, so only the destructor's dead vtable stores are affected).
class CThingCreatureBase;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThingCreatureBase* m_pItem;            // +0x04
    CThingCreatureBase* GetPItem() const;   // 0x00A01B50
    ~CBaseIntelligentPointer();             // 0x00A01C10
};

struct __declspec(novtable) CIntelligentPointer : CBaseIntelligentPointer {   // implicit destructor: no vtable store of its own
    virtual void v00();
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4, 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10 rest of CCreatureActionBase (0xa8 bytes in retail)
    CBaseIntelligentPointer PTarget;        // +0xa8 CIntelligentPointer<CThing>
    unsigned char _padb0[0x114 - 0xb0];     // +0xb0
    ~CCombatActionBase();                   // 0x0062E900
};

struct CThingCreatureBase_Calls {
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};

struct __declspec(novtable) CCreatureAction_BriarRoseShotFarOutof : CCombatActionBase {   // sizeof 0x11c; constructed out of line
    CIntelligentPointer PTarget;            // +0x114 (the member its constructor @ 0x008C39A0 builds)
    virtual void v00();
    CCreatureAction_BriarRoseShotFarOutof(CThingCreatureBase* creature, CThingCreatureBase* target);   // 0x008C39A0
};

class CCreatureAction_BriarRoseShotFarInto : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_BriarRoseShotFarInto::FinishAction()
{
    CThingCreatureBase* creature = PCreature.GetPItem();
    CThingCreatureBase* target = PTarget.GetPItem();
    CCombatActionBase::FinishAction();
    ((CThingCreatureBase_Calls*)creature)->SetCurrentAction(CCreatureAction_BriarRoseShotFarOutof(creature, target));
}