// CCombatAction_WillForcePushInto::FinishAction() -- base FinishAction, then hand the creature a temporary CCreatureAction_CastWillForceSpell(creature, 1, true, "ST_FORCE_PUSH_RELEASE") as its current action
// Leaf: ?FinishAction@CCombatAction_WillForcePushInto
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C2980, 112-byte slot (97 bytes of code). FinishAction is vtable slot 4.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, fetched into edi BEFORE the base call:
//          a named local), CCombatActionBase::FinishAction @ 0x00859A20 (direct base call),
//          CCharString::CCharString(const char*, long) @ 0x0099EBF0,
//          CCreatureAction_CastWillForceSpell::CCreatureAction_CastWillForceSpell(CThingCreatureBase*, long, bool, const CCharString&)
//          @ 0x008BB200, CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) @ 0x006644F0,
//          the action temporary's destructor @ 0x0062E900 (manifest "~CTCLightningOrb" is propagated),
//          CCharString::~CCharString @ 0x0099EAE0.
// Shape: the landed WillEnflameInto::FinishAction (0x008C2A80) with a named animation string: the 0x120-byte frame
// holds the string word below the 0x11c-byte action temporary; the temporary dies first, then the string.
struct CCreatureActionBase;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CThingCreatureBase_Calls {
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

struct CCreatureAction_CastWillForceSpell_Retail {      // the retail temporary is 0x11c bytes
    unsigned char _body[0x11c];
    CCreatureAction_CastWillForceSpell_Retail(CThingCreatureBase_Calls* creature, long level, bool release,
                                              const CCharString& animName);   // 0x008BB200
    ~CCreatureAction_CastWillForceSpell_Retail();                            // 0x0062E900
    operator const CCreatureActionBase&() const { return *reinterpret_cast<const CCreatureActionBase*>(this); }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThingCreatureBase_Calls* GetPItem() const; // 0x00A01B50
};

struct CCombatActionBase {
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char _pad10[0x114 - 0x10];         // +0x10
    void FinishAction();                        // 0x00859A20
};

struct CCombatAction_WillForcePushInto : CCombatActionBase {
    void FinishAction();
};

void CCombatAction_WillForcePushInto::FinishAction()
{
    CThingCreatureBase_Calls* creature = PCreature.GetPItem();
    CCombatActionBase::FinishAction();
    CCharString animName("ST_FORCE_PUSH_RELEASE", -1);
    creature->SetCurrentAction(CCreatureAction_CastWillForceSpell_Retail(creature, 1, true, animName));
}