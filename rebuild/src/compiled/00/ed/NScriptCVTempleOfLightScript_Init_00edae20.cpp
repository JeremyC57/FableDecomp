// NScript::CV_TempleOfLightScript::Init() -- reset the Temple of Light donation, morality and quest state
// Leaf: ?Init@CV_TempleOfLightScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EDAE20, 72 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 (Init) of the level-script vtable 0x012F3C68 (slot 1 = `NScript::CV_TempleOfLightScript::RegisterMain`), stored by 0x00EDADF0; ?Init@CV_TempleOfLightScript@NScript@@UAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00edae20" is a gap-scan placeholder.
// Layout: retail CScriptBase is 0x40 bytes (Ego_r.pdb 0x54), then the interface +0x40 (and the master data +0x44 where the ctor takes one); the script's own members sit 0x14 below their Ego_r.pdb offsets: DonationTotal +0x48 .. QuestActivated +0x72.
// Shape: the three float members are zeroed through the zero register (`mov [ecx+N],eax`).
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString();                                        // 0x0099E4B0
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
};
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TempleOfLightScript : public CScriptBase {
public:
    virtual void Init();
    CGameScriptInterfaceBase* Game;                     // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    long DonationTotal;                                 // +0x48
    long DonationThisVisit;                             // +0x4c
    long DonationCounter;                               // +0x50
    bool DonationInvalid;                               // +0x54
    bool DonationActive;                                // +0x55
    bool DisplayQuestInfo;                              // +0x56
    bool WeaponWarningComplete;                         // +0x57
    bool WeaponUnsheathed;                              // +0x58
    bool TempleIntroComplete;                           // +0x59
    bool InsideTemple;                                  // +0x5a
    bool AttackedByHero;                                // +0x5b
    bool AcolyteAttackMessageComplete;                  // +0x5c
    bool AllAcolytesEscaped;                            // +0x5d
    bool ReceivedReward;                                // +0x5e
    bool GivenCoolThing1;                               // +0x5f
    bool GivenCoolThing2;                               // +0x60
    bool AwardWeapon;                                   // +0x61
    float MoralityInitial;                              // +0x64
    float MoralityCurrent;                              // +0x68
    float MoralityCumulative;                           // +0x6c
    bool QuestFinished;                                 // +0x70
    bool QuestComplete;                                 // +0x71
    bool QuestActivated;                                // +0x72
};
}

void NScript::CV_TempleOfLightScript::Init()
{
    DonationTotal = 0;
    DonationThisVisit = 0;
    DonationCounter = 0;
    DonationInvalid = false;
    DonationActive = false;
    DisplayQuestInfo = false;
    WeaponWarningComplete = false;
    WeaponUnsheathed = false;
    TempleIntroComplete = false;
    InsideTemple = false;
    AttackedByHero = false;
    AcolyteAttackMessageComplete = false;
    AllAcolytesEscaped = false;
    MoralityInitial = 0.0f;
    MoralityCurrent = 0.0f;
    MoralityCumulative = 0.0f;
    QuestFinished = false;
    QuestActivated = false;
    QuestComplete = false;
    ReceivedReward = false;
    GivenCoolThing1 = false;
    GivenCoolThing2 = false;
    AwardWeapon = false;
}