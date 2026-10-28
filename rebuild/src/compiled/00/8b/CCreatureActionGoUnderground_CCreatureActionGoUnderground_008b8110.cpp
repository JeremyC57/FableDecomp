// CCreatureAction_GoUnderground::CCreatureAction_GoUnderground(CThingCreatureBase*) -- combat action with an unnamed sound pair (action code 220), animation "GO_UNDERGROUND"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B8110, length 116 bytes, `ret 4`. Vtable 0x0127A894.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair (__fastcall) @ 0x008561A0,
//          CCombatActionBase::CCombatActionBase(owner, actionCode, const CSoundPair&) @ 0x00858030,
//          CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0
//          (manifest label "CTCInGameMenu::SetPDef" is propagated; the name is the Ego_r/FableWin PDB one).
// Shape: the sound name is a NULL pointer converted implicitly (`push -1 ; push 0`), the hidden-pointer CSoundPair
//   feeds the base constructor, the string dies after it; then a scoped named CCharString for the animation (its
//   constructor pushes are scheduled above the vtable store, GOTCHAS family wave) and the setter call.
// Layout: engine/CCreatureAction_GoUnderground.h is the PDB build; this constructor touches no member of its own.
class CThingCreatureBase;

class CCharString { public:                        // one pointer; the defaulted length makes a literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CSoundPair { public:                         // user constructor: the 8-byte result comes back through the hidden pointer
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name);   // 0x008561A0

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    void SetCombatAnimation(const CCharString& name);                                           // 0x008574D0
};

class CCreatureAction_GoUnderground : public CCombatActionBase { public:   // vtable 0x0127A894
    virtual void v00();
    CCreatureAction_GoUnderground(CThingCreatureBase* owner);
};

CCreatureAction_GoUnderground::CCreatureAction_GoUnderground(CThingCreatureBase* owner)
    : CCombatActionBase(owner, 220, GetSoundPair(owner, (const char*)0))
{
    {
        CCharString name("GO_UNDERGROUND", -1);
        SetCombatAnimation(name);
    }
}