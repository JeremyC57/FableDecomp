// CCreatureAction_ScorpionRecover::CCreatureAction_ScorpionRecover(CThingCreatureBase*) -- combat action with the SWORD_SWING sound pair (action code 150), animation "RECOVER"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E4770, length 119 bytes, `ret 4`. Vtable 0x01286724.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair (__fastcall) @ 0x008561A0,
//          CCombatActionBase::CCombatActionBase(owner, actionCode, const CSoundPair&) @ 0x00858030,
//          CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0
//          (manifest label "CTCInGameMenu::SetPDef" is propagated; the name is the Ego_r/FableWin PDB one).
// Shape: the "SWORD_SWING" literal converts implicitly to a CCharString temporary, the hidden-pointer CSoundPair
//   feeds the base constructor, the string dies after it; then a scoped named CCharString for the animation (its
//   constructor pushes are scheduled above the vtable store, GOTCHAS family wave) and the setter call.
// Identity: the stored vtable 0x01286724 is CCreatureAction_ScorpionRecover (rtti-vtables.tsv); the catalog label
//   "CCreatureAction_GoUnderground" is propagated over the identical body shape. No member of its own is touched.
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

class CCreatureAction_ScorpionRecover : public CCombatActionBase { public:   // vtable 0x01286724
    virtual void v00();
    CCreatureAction_ScorpionRecover(CThingCreatureBase* owner);
};

CCreatureAction_ScorpionRecover::CCreatureAction_ScorpionRecover(CThingCreatureBase* owner)
    : CCombatActionBase(owner, 150, GetSoundPair(owner, "SWORD_SWING"))
{
    {
        CCharString name("RECOVER", -1);
        SetCombatAnimation(name);
    }
}