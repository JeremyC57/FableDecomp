// NScript::CV_ChapelOfEvilScript::Init() -- reset the Chapel of Evil quest state: donations, flags and morality tallies cleared, torture count 1, no torture sound
// Leaf: ?Init@CV_ChapelOfEvilScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E68FA0, 131 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012E8080, which the constructor 0x00E68F70 stores
//   (landed under the stand-in name CVtable_012E8080::CVtable_012E8080); its Alloc 0x00E6E230 (landed as
//   _global::Alloc_CVtable_012e8080) news 0x90 bytes (Ego_r.pdb CV_ChapelOfEvilScript 0xA4 - 0x14) and is registered as
//   "S_VCOE" (0x00CD85CE); every store hits an Ego_r.pdb CV_ChapelOfEvilScript member at Ego_r - 0x14. Level-script
//   slots: 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist. Ego_r.pdb has ?Init@CV_ChapelOfEvilScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_ChapelOfEvilScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CCharString::operator=(const char*) 0x0099EFE0 ("" 0x0122D70E). Stores in retail order (the source order);
//   the float zeros are integer stores of the zero register.
class CCharString {
public:
    void* PData;
    const CCharString& operator=(const char* text);     // 0x0099EFE0
};
enum ESex { };
enum ChapelOfEvilTortureType { };
class CGameScriptInterface;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ChapelOfEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long DonationTotal;                                 // +0x48
    long DonationThisVisit;                             // +0x4c
    long DonationCounter;                               // +0x50
    bool DonationInvalid;                               // +0x54
    bool DisplayQuestInfo;                              // +0x55
    bool WeaponWarningComplete;                         // +0x56
    bool WeaponUnsheathed;                              // +0x57
    bool ChapelIntroComplete;                           // +0x58
    bool InsideChapel;                                  // +0x59
    bool AttackedByHero;                                // +0x5a
    bool AcolyteAttackMessageComplete;                  // +0x5b
    bool ReceivedReward;                                // +0x5c
    bool GivenCoolThing1;                               // +0x5d
    bool GivenCoolThing2;                               // +0x5e
    bool AwardWeapon;                                   // +0x5f
    float MoralityInitial;                              // +0x60
    float MoralityCurrent;                              // +0x64
    float MoralityCumulative;                           // +0x68
    bool QuestFinished;                                 // +0x6c
    bool QuestComplete;                                 // +0x6d
    bool QuestActivated;                                // +0x6e
    long TortureCount;                                  // +0x70
    bool VictimTortured;                                // +0x74
    long VictimHealth;                                  // +0x78
    ESex VictimGender;                                  // +0x7c
    bool VictimNightTime;                               // +0x80
    bool VictimSunday;                                  // +0x81
    bool VictimIsGuard;                                 // +0x82
    bool VictimIsBandit;                                // +0x83
    bool VictimIsTrader;                                // +0x84
    CCharString TortureSound;                           // +0x88
    ChapelOfEvilTortureType TortureType;                // +0x8c
    virtual void Init();
};

void CV_ChapelOfEvilScript::Init()
{
    DonationTotal = 0;
    DonationThisVisit = 0;
    DonationCounter = 0;
    DonationInvalid = false;
    DisplayQuestInfo = false;
    WeaponWarningComplete = false;
    WeaponUnsheathed = false;
    ChapelIntroComplete = false;
    InsideChapel = false;
    AttackedByHero = false;
    AcolyteAttackMessageComplete = false;
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
    TortureCount = 1;
    VictimTortured = false;
    VictimHealth = 0;
    VictimNightTime = false;
    VictimSunday = false;
    VictimIsGuard = false;
    VictimIsBandit = false;
    VictimIsTrader = false;
    TortureType = (ChapelOfEvilTortureType)0;
    TortureSound = "";
}
}