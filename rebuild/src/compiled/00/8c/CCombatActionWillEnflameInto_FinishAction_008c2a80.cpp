// CCombatAction_WillEnflameInto::FinishAction() -- base FinishAction, then hand the creature a temporary CCreatureAction_CastWillEnflameSpell(creature, Level) as its current action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C2A80, length 70 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, fetched into edi BEFORE the base
// call: a named local), CCombatActionBase::FinishAction @ 0x00859A20 (base virtual, non-virtual call),
// CCreatureAction_CastWillEnflameSpell::CCreatureAction_CastWillEnflameSpell(CThingCreatureBase*, long)
// @ 0x008BB450 (the 0x11c-byte temporary at the bottom of the frame), CThingCreatureBase::SetCurrentAction
// (const CCreatureActionBase&) @ 0x006644F0 (the temporary passed by reference: `push eax` of the constructor's
// returned this), its destructor @ 0x0062E900 (manifest "~CTCLightningOrb" is a propagated label).
// Level is the long at +0x114 (Ego_r.pdb name; the engine header has it at +0x12c for the PDB build, retail's
// CCombatActionBase is 0x114 bytes, so the layout is modelled locally).
struct CCreatureActionBase;
struct CThingCreatureBase_Calls {              // engine/CThingCreatureBase.h exists but is not used: only its SetCurrentAction call is used
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

struct CCreatureAction_CastWillEnflameSpell_Retail {              // engine/CCreatureAction_CastWillEnflameSpell.h exists but is not used: the retail temporary is 0x11c bytes (the PDB build's header differs: base 0x12c); only its ctor/dtor and size matter here
    unsigned char _body[0x11c];
    CCreatureAction_CastWillEnflameSpell_Retail(CThingCreatureBase_Calls* creature, long level);   // 0x008BB450
    ~CCreatureAction_CastWillEnflameSpell_Retail();                                          // 0x0062E900
    operator const CCreatureActionBase&() const { return *reinterpret_cast<const CCreatureActionBase*>(this); }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThingCreatureBase_Calls* GetPItem() const;       // 0x00A01B50
};

struct CCombatActionBase {
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char _pad10[0x114 - 0x10];         // +0x10
    void FinishAction();                        // 0x00859A20
};

struct CCombatAction_WillEnflameInto : CCombatActionBase {
    long Level;                                 // +0x114
    void FinishAction();
};

void CCombatAction_WillEnflameInto::FinishAction()
{
    CThingCreatureBase_Calls* creature = PCreature.GetPItem();
    CCombatActionBase::FinishAction();
    creature->SetCurrentAction(CCreatureAction_CastWillEnflameSpell_Retail(creature, Level));
}