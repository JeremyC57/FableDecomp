// CCreatureAction_JackScream::CCreatureAction_JackScream(CThingCreatureBase*) -- base CCombatActionBase(owner, 250, GetSoundPair(owner, "SWORD_SWING")), an empty mode pointer, animation "SCREAM_INTO"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00977100, length 133 bytes (+ int3 padding). Vtable 0x01297CB4.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair @ 0x008561A0, CCombatActionBase::CCombatActionBase(
// CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030, CCharString::~CCharString @ 0x0099EAE0, SetCombatAnimation @ 0x008574D0.
// Shape: the one-argument combat-action family; after the vtable store the two words of PModeCountedPointer (CCountedPointer<CModeJackOfBladesScream>,
// Data/Info, PDB +0x12c/+0x130 = retail +0x114/+0x118) are zeroed through one register by the member's inline constructor; the name is a scoped local.
class CThingCreatureBase;
class CThing;

class CCharString { public:                        // one pointer; the default length makes a literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
class CSoundPair { public:                         // a user constructor makes the 8-byte result return through the hidden pointer
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    // NF123 (2026-11-20, q6403): Ego_r.pdb ?GetSoundPair@CCombatActionBase@@KI?AVCSoundPair@@ABVCThingCreatureBase@@ABVCCharString@@@Z
    //   (protected static __fastcall, 0x008561A0; engine/CCombatActionBase.h SOUNDCTOR); was a free GetSoundPair(CThingCreatureBase*, ...).
protected:
    static CSoundPair __fastcall GetSoundPair(const CThingCreatureBase& creature, const CCharString& name);   // 0x008561A0
public:
    virtual void v00();
    unsigned char _body[0x114 - 4];
    // NF123 (2026-11-20, q6404): creature by reference, Ego_r.pdb ??0CCombatActionBase@@QAE@AAVCThingCreatureBase@@JABVCSoundPair@@@Z (0x00858030; was PAV).
    CCombatActionBase(CThingCreatureBase& owner, long actionCode, const CSoundPair& sound);                  // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};
struct CModeJackOfBladesScream;
class CCPPointerInfo;
struct CScreamModePointer {                 // CCountedPointer<NCreatureMode::CModeJackOfBladesScream>
    CModeJackOfBladesScream* Data;
    CCPPointerInfo* Info;
    CScreamModePointer() : Data(0), Info(0) { }
};
class CCreatureAction_JackScream : public CCombatActionBase { public:   // vtable 0x01297CB4
    CScreamModePointer PModeCountedPointer; // +0x114
    virtual void v00();
    CCreatureAction_JackScream(CThingCreatureBase* owner);
};

CCreatureAction_JackScream::CCreatureAction_JackScream(CThingCreatureBase* owner)
    : CCombatActionBase(*owner, 250, GetSoundPair(*owner, "SWORD_SWING"))
{
    {
        CCharString name("SCREAM_INTO", -1);
        SetCombatAnimation(name);
    }
}