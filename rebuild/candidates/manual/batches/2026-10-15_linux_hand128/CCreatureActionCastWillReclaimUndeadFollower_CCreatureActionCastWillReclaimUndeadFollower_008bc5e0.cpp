// CCreatureAction_CastWillReclaimUndeadFollower::CCreatureAction_CastWillReclaimUndeadFollower(CThingCreatureBase*, CThingGameObject*) -- will-spell combat action (action code 150, silent sound pair) holding the undead follower to reclaim, animation "WILL_CAST_FORCE_SPELL_DELIVER"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BC5E0, length 123 bytes, `ret 8`. Vtable 0x0127D5BC.
// Callees: CCombatActionBase::CCombatActionBase(owner, actionCode, const CSoundPair&) @ 0x00858030,
//          CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90,
//          CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName @ 0x008574D0
//          (manifest label "CTCInGameMenu::SetPDef" is propagated), CCharString::~CCharString @ 0x0099EAE0.
// Shape: the CSoundPair temporary zeroed from eax by its inline constructor; after the vtable store the
//   PUndeadFollower member (addressed once in edi) runs the out-of-line base constructor, its own vtable store
//   (CIntelligentPointer<CThingGameObject> 0x0127B4D8) and SetPItem(follower) -- an inline converting constructor;
//   then the scoped named animation string and the setter.
// Layout: Ego_r.pdb puts PUndeadFollower at +0x12c; the retail CCombatActionBase is 0x114 bytes, so it sits at +0x114.
struct CThingCreatureBase;
struct CThingGameObject;
struct CBaseObject;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CBaseIntelligentPointer {            // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                   // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingGameObject>: vtable 0x0127B4D8
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    void SetAnimationName(const CCharString& name);                                           // 0x008574D0
};

struct CCreatureAction_CastWillReclaimUndeadFollower : CCombatActionBase {   // vtable 0x0127D5BC
    CIntelligentPointer<CThingGameObject> PUndeadFollower;   // +0x114 (PDB +0x12c)
    virtual void v00();
    CCreatureAction_CastWillReclaimUndeadFollower(CThingCreatureBase* owner, CThingGameObject* follower);
};

CCreatureAction_CastWillReclaimUndeadFollower::CCreatureAction_CastWillReclaimUndeadFollower(CThingCreatureBase* owner, CThingGameObject* follower)
    : CCombatActionBase(owner, 150, CSoundPair())
    , PUndeadFollower(follower)
{
    {
        CCharString name("WILL_CAST_FORCE_SPELL_DELIVER", -1);
        SetAnimationName(name);
    }
}
