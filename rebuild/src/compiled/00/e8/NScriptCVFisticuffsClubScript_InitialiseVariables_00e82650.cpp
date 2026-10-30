// CV_FisticuffsClubScript::InitialiseVariables() -- reset the fight state, re-arm both fighter timers with the
// script-def fighter delay, recount the fights, mark the club active unless only-at-night applies, and name the four
// club regions
// Leaf: ?InitialiseVariables@CV_FisticuffsClubScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E82650, 333 bytes (padded to 336).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0 / operator=(const char*) @ 0x0099EFE0,
//          CScriptBase::IsActiveThreadTerminating @ 0x00CB7940, SetFightCount @ 0x00E82800, the second set-up
//          member @ 0x00E827A0 (name unknown; labelled CScriptBase_InitialiseVariables_Check); CGameScriptInterface
//          slots 89 SetTimer (+0x164, through g_pGameScriptInterface 0x0143E8F8), 284 IsQuestActive (+0x470).
// Layout (retail = Ego_r.pdb CV_FisticuffsClubScript - 0x14): flags +0x4c..+0x58, Fighter1Timer +0x5c, Fighter2Timer
//          +0x60, HitID +0x7c, HitCount +0x80, AttackWarnings +0x84, EverybodyAttack +0x8c, LostWhileOutside +0x8d,
//          FisticuffsActive +0x8e, AllowedHits +0x90, PlayerLost +0xa8, CheerNumber +0xac, BooNumber +0xb0,
//          RegionNames[5] +0xb4; script def (0x0143E90C) FC_FighterDelay +0x5dc (Ego_r.pdb +0x638: retail = PDB -
//          4 x preceding vectors - 4).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    const CCharString& operator=(const char* text);            // 0x0099EFE0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};

template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    ~CScriptThing() {}                                   // out of line 0x004AA840
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};
class CScriptDef { public:
    unsigned char _pad[0x5dc];
    long          FC_FighterDelay;                       // +0x5dc
};
extern CScriptDef*           pGlobalScriptDef;               // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
extern CGameScriptInterface* g_pGameScriptInterface;     // 0x0143E8F8
class CTimer { public:                                          // a script timer: the interface's timer index
    long Index;
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
};
namespace NScript {
class CV_FisticuffsClubScript : public CQuestScriptBase {
public:
    long          RelevantRegion;                        // +0x48
    bool          FighterOneInRing;                      // +0x4c
    bool          FighterTwoInRing;                      // +0x4d
    bool          ChallengeAccepted;                     // +0x4e
    bool          FightFinished;                         // +0x4f
    bool          HeroInsideRing;                        // +0x50
    bool          HeroOfferedFisticuffsClubEntry;        // +0x51
    bool          HeroPaidEntryFee;                      // +0x52
    bool          HeroInFisticuffsClub;                  // +0x53
    bool          TylerInFight;                          // +0x54
    bool          HeroBeatTyler;                         // +0x55
    bool          TimeOut;                               // +0x56
    bool          RingCleared;                           // +0x57
    bool          RoundStarted;                          // +0x58
    CTimer        Fighter1Timer;                         // +0x5c
    CTimer        Fighter2Timer;                         // +0x60
    long          SFighterOne[3];                        // +0x64 (CScriptThing)
    long          SFighterTwo[3];                        // +0x70 (CScriptThing)
    long          HitID;                                 // +0x7c
    long          HitCount;                              // +0x80
    long          AttackWarnings;                        // +0x84
    long          FightCount;                            // +0x88
    bool          EverybodyAttack;                       // +0x8c
    bool          LostWhileOutside;                      // +0x8d
    bool          FisticuffsActive;                      // +0x8e
    long          AllowedHits;                           // +0x90
    bool          CompletedThisPunchClub;                // +0x94
    bool          CompletedKnotholePunchClub;            // +0x95
    bool          CompletedBowerstonePunchClub;          // +0x96
    bool          CompletedBanditPunchClub;              // +0x97
    bool          CompletedOakvalePunchClub;             // +0x98
    long          Village[3];                            // +0x9c (CScriptThing)
    bool          PlayerLost;                            // +0xa8
    float         CheerNumber;                           // +0xac
    float         BooNumber;                             // +0xb0
    CCharString   RegionNames[5];                        // +0xb4
    void SetFightCount();                                // 0x00E82800
    void InitialiseVariablesCheck();                     // 0x00E827A0 (name unknown)
    void InitialiseVariables();
};
} // namespace NScript
using NScript::CV_FisticuffsClubScript;

void NScript::CV_FisticuffsClubScript::InitialiseVariables()
{
    FighterOneInRing = false;
    FighterTwoInRing = false;
    ChallengeAccepted = false;
    FightFinished = true;
    HeroOfferedFisticuffsClubEntry = false;
    HeroPaidEntryFee = false;
    HeroInsideRing = false;
    HeroInFisticuffsClub = false;
    TylerInFight = false;
    HeroBeatTyler = false;
    RingCleared = true;
    RoundStarted = false;
    Fighter1Timer.SetTime(pGlobalScriptDef->FC_FighterDelay);
    Fighter2Timer.SetTime(pGlobalScriptDef->FC_FighterDelay);
    CheerNumber = 0;
    BooNumber = 0;
    TimeOut = false;
    HitID = 0;
    PlayerLost = false;
    HitCount = 0;
    SetFightCount();
    InitialiseVariablesCheck();
    AttackWarnings = 0;
    EverybodyAttack = false;
    LostWhileOutside = false;
    bool onlyAtNight = GSI->IsQuestActive("V_FisticuffsClubOnlyAtNight");
    if (onlyAtNight) {
        if (IsActiveThreadTerminating())
            return;
        FisticuffsActive = true;
    } else {
        if (IsActiveThreadTerminating())
            return;
        FisticuffsActive = false;
    }
    AllowedHits = 0;
    RegionNames[1] = "BowerstoneSlums";
    RegionNames[2] = "Oakbay";
    RegionNames[3] = "BanditCampCentre";
    RegionNames[4] = "KnotholeGlade";
}