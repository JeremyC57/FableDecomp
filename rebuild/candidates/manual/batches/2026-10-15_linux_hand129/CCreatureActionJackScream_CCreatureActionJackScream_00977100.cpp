// CCreatureAction_JackScream::CCreatureAction_JackScream(CThingCreatureBase*) -- base CCombatActionBase(owner, 250, GetSoundPair(owner, "SWORD_SWING")), an empty mode pointer, animation "SCREAM_INTO"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00977100, length 133 bytes (+ int3 padding). Vtable 0x01297CB4.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair @ 0x008561A0, CCombatActionBase::CCombatActionBase(
// CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030, CCharString::~CCharString @ 0x0099EAE0, SetAnimationName @ 0x008574D0.
// Shape: the one-argument combat-action family; after the vtable store the two words of PModeCountedPointer (CCountedPointer<CModeJackOfBladesScream>,
// Data/Info, PDB +0x12c/+0x130 = retail +0x114/+0x118) are zeroed through one register by the member's inline constructor; the name is a scoped local.
struct CThingCreatureBase;
struct CThing;

struct CCharString {                        // one pointer; the default length makes a literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
struct CSoundPair {                         // a user constructor makes the 8-byte result return through the hidden pointer
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name);   // 0x008561A0

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                  // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};
struct CModeJackOfBladesScream;
struct CCPPointerInfo;
struct CScreamModePointer {                 // CCountedPointer<NCreatureMode::CModeJackOfBladesScream>
    CModeJackOfBladesScream* Data;
    CCPPointerInfo* Info;
    CScreamModePointer() : Data(0), Info(0) { }
};
struct CCreatureAction_JackScream : CCombatActionBase {   // vtable 0x01297CB4
    CScreamModePointer PModeCountedPointer; // +0x114
    virtual void v00();
    CCreatureAction_JackScream(CThingCreatureBase* owner);
};

CCreatureAction_JackScream::CCreatureAction_JackScream(CThingCreatureBase* owner)
    : CCombatActionBase(owner, 250, GetSoundPair(owner, "SWORD_SWING"))
{
    {
        CCharString name("SCREAM_INTO", -1);
        SetAnimationName(name);
    }
}
