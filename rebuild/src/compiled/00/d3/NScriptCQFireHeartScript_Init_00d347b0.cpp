// CQ_FireHeartScript::Init() -- register the demon-door lookout region and reset the quest state: flags, the pad FX names, the starting sun count by the hero's morality, the puzzle patterns and counts from the script def, the OPENCAGE cut-scene flag, the per-pad / per-prophet / per-villager bookkeeping vectors and the morality changes
// Leaf: ?Init@CQ_FireHeartScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D347B0, length 779 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 / operator=(const CCharString&) 0x0099EFB0,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, vector<CFireHeartPatternDef>::operator= 0x00D34D40,
//   map<CCharString, bool>::operator[] 0x008ADF10, vector<bool>::resize 0x00467686, CScriptThing() 0x006E7B40 /
//   ~CScriptThing 0x004AA840, vector<CScriptThing>::resize 0x00D34AC0, scripted-thing handle ctor 0x007E72A0 / dtor
//   0x007E74D0, vector<CScriptGameResourceObjectScriptedThingBase>::resize 0x00CD3CCB; through the interface (+0x40):
//   GetHeroMorality (157), AddQuestRegion (271), GetActiveQuestName (655).
// Data: pGlobalScriptDef 0x0143E90C FHSunPadFX +0xC04, FHMoonPadFX +0xC08, FHPadRadius +0xC10, FHNumberOfPads +0xC14,
//   FHNumberOfProphets +0xC18, FHNumberOfPatterns +0xC1C, FHNumberOfRounds +0xC20, FHNumberOfSunsEvil +0xC24,
//   FHNumberOfSunsGood +0xC28, FHPatterns +0xC2C, FHNumberOfVillagers +0xC48, FHMoralityChangeOnKilled +0xC50,
//   FHMoralityChangeOnFreed +0xC54.
// Strings: "DemonDoor_LookoutPoint" 0x012CCC34, "OPENCAGE" 0x012CCC28.
// Layout (retail = Ego_r.pdb - 0x14; each earlier STLport map / vector 4 bytes smaller, vector<bool> a plain 12-byte
//   vector): see the member comments.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
namespace N_006e7b40 { struct CScriptThing {                                   // 12 bytes, vtable 0x01238C8C
    void* _vfptr;
    void* PImp_Data;
    void* PImp_Info;
    CScriptThing();                                     // 0x006E7B40
    ~CScriptThing();                                    // 0x004AA840
}; } using N_006e7b40::CScriptThing;
class CScriptGameResourceObjectScriptedThingBase { public:    // 0x10 bytes, vtable 0x0127094C
    unsigned char _body[0x10 - 4];
    CScriptGameResourceObjectScriptedThingBase();       // 0x007E72A0
    virtual ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
};
class CFireHeartPatternDef;
struct CFireHeartPatternVector {                        // vector<CFireHeartPatternDef> (retail 12 bytes)
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    CFireHeartPatternVector& operator=(const CFireHeartPatternVector& other);   // 0x00D34D40
};
struct CBoolVector {                                    // vector<bool> (retail: a plain 12-byte vector)
    bool* _M_start; bool* _M_finish; bool* _M_end_of_storage;
    void resize(unsigned int n, const bool& x);         // 0x00467686
};
struct CScriptThingVector {                             // vector<CScriptThing> (retail 12 bytes)
    CScriptThing* _M_start; CScriptThing* _M_finish; CScriptThing* _M_end_of_storage;
    void resize(unsigned int n, const CScriptThing& x); // 0x00D34AC0
    void resize(unsigned int n) { resize(n, CScriptThing()); }
};
struct CScriptedThingVector {                           // vector<CScriptGameResourceObjectScriptedThingBase> (retail 12 bytes)
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    void resize(unsigned int n, const CScriptGameResourceObjectScriptedThingBase& x);   // 0x00CD3CCB
    void resize(unsigned int n) { resize(n, CScriptGameResourceObjectScriptedThingBase()); }
};
struct CCutsceneFlagMap {                               // map<CCharString, bool> (retail 12 bytes)
    void* _body[3];
    bool& operator[](const CCharString& key);           // 0x008ADF10
};
struct CCutsceneArgMap { void* _body[3]; };            // map<CCharString, CCharString> (retail 12 bytes)
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:
    unsigned char _pad[0xc04];
    CCharString FHSunPadFX;                             // +0xC04
    CCharString FHMoonPadFX;                            // +0xC08
    long        _padc0c;
    float       FHPadRadius;                            // +0xC10
    long        FHNumberOfPads;                         // +0xC14
    long        FHNumberOfProphets;                     // +0xC18
    long        FHNumberOfPatterns;                     // +0xC1C
    long        FHNumberOfRounds;                       // +0xC20
    long        FHNumberOfSunsEvil;                     // +0xC24
    long        FHNumberOfSunsGood;                     // +0xC28
    CFireHeartPatternVector FHPatterns;                 // +0xC2C
    unsigned char _padc38[0xc48 - 0xc38];
    long        FHNumberOfVillagers;                    // +0xC48
    long        _padc4c;
    float       FHMoralityChangeOnKilled;               // +0xC50
    float       FHMoralityChangeOnFreed;                // +0xC54
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
enum EMissionHistory { MISSION_HISTORY_NONE };
class CQ_FireHeartScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    bool QuestStartScreenShown;                         // +0x48
    bool MissionSucceeded;                              // +0x49
    bool MissionFailed;                                 // +0x4a
    CCutsceneFlagMap CutsceneFlags;                     // +0x4c
    CCutsceneArgMap  csargs;                            // +0x58
    long FreedProphets;                                 // +0x64
    long KilledProphets;                                // +0x68
    long StartNumberOfSuns;                             // +0x6c
    long NumberOfRounds;                                // +0x70
    long NumberOfPatterns;                              // +0x74
    long NumberOfProphets;                              // +0x78
    long NumberOfPads;                                  // +0x7c
    long NumberOfSuns;                                  // +0x80
    long NumberOfVillagers;                             // +0x84
    long PuzzleRound;                                   // +0x88
    bool ResetBlocks;                                   // +0x8c
    long NumBlocksReset;                                // +0x90
    bool RoundEnded;                                    // +0x94
    bool RoundBeginning;                                // +0x95
    bool PreventRoundEnd;                               // +0x96
    bool HasHeroHitVillagersGroupOne;                   // +0x97
    bool HasHeroHitVillagersGroupTwo;                   // +0x98
    bool InfoDisplayed;                                 // +0x99
    long EvilRounds;                                    // +0x9c
    long GoodRounds;                                    // +0xa0
    float PadRadius;                                    // +0xa4
    CScriptThing Effect;                                // +0xa8
    CCharString PadSunFX;                               // +0xb4
    CCharString PadMoonFX;                              // +0xb8
    CBoolVector StartingPattern;                        // +0xbc
    CBoolVector ProphetsKilled;                         // +0xc8
    CBoolVector ProphetsReleased;                       // +0xd4
    CBoolVector ShouldProphetSpeak;                     // +0xe0
    CBoolVector ShouldVillagerSpeak;                    // +0xec
    CBoolVector CommentGivenThisRound;                  // +0xf8
    CScriptThingVector Prophets;                        // +0x104
    CScriptThingVector Villagers;                       // +0x110
    CScriptThingVector Followers;                       // +0x11c
    CScriptedThingVector FollowerHandles;               // +0x128
    CFireHeartPatternVector Pattern;                    // +0x134
    long Guest;                                         // +0x140
    long ProphetsScripted;                              // +0x144
    CScriptedThingVector seh_Prophet;                   // +0x148
    CScriptGameResourceObjectScriptedThingBase seh_BriarRose;     // +0x154
    CScriptGameResourceObjectScriptedThingBase seh_Scythe;        // +0x164
    CScriptGameResourceObjectScriptedThingBase seh_GuildMaster;   // +0x174
    EMissionHistory MissionHistory;                     // +0x184
    CCharString ProphetComment;                         // +0x188
    CScriptThing VillagerEscapeMarker;                  // +0x18c
    float MoralityChangeOnKilled;                       // +0x198
    float MoralityChangeOnFreed;                        // +0x19c
    bool HasDemonDoorBeenEnabled;                       // +0x1a0
    bool HasScytheCutscenePlayed;                       // +0x1a1
    bool KilledAllPrisoners;                            // +0x1a2
    bool FreedAllPrisoners;                             // +0x1a3
    void Init();
};
void CQ_FireHeartScript::Init()
{
    Game->AddQuestRegion(Game->GetActiveQuestName(), "DemonDoor_LookoutPoint");
    QuestStartScreenShown = false;
    MissionFailed = false;
    MissionSucceeded = false;
    RoundEnded = false;
    RoundBeginning = false;
    PreventRoundEnd = false;
    ResetBlocks = false;
    HasHeroHitVillagersGroupOne = false;
    HasHeroHitVillagersGroupTwo = false;
    InfoDisplayed = false;
    PadSunFX = pGlobalScriptDef->FHSunPadFX;
    PadMoonFX = pGlobalScriptDef->FHMoonPadFX;
    if (Game->GetHeroMorality() < 0.5f) {
        if (IsActiveThreadTerminating())
            return;
        StartNumberOfSuns = pGlobalScriptDef->FHNumberOfSunsEvil;
    } else {
        if (IsActiveThreadTerminating())
            return;
        StartNumberOfSuns = pGlobalScriptDef->FHNumberOfSunsGood;
    }
    Pattern = pGlobalScriptDef->FHPatterns;
    PadRadius = pGlobalScriptDef->FHPadRadius;
    NumberOfRounds = pGlobalScriptDef->FHNumberOfRounds;
    NumberOfPatterns = pGlobalScriptDef->FHNumberOfPatterns;
    NumberOfPads = pGlobalScriptDef->FHNumberOfPads;
    NumberOfSuns = 0;
    PuzzleRound = 0;
    CutsceneFlags["OPENCAGE"] = false;
    FreedProphets = 0;
    KilledProphets = 0;
    NumberOfProphets = pGlobalScriptDef->FHNumberOfProphets;
    NumberOfVillagers = pGlobalScriptDef->FHNumberOfVillagers;
    StartingPattern.resize(NumberOfPads, false);
    ProphetsKilled.resize(NumberOfProphets, false);
    ProphetsReleased.resize(NumberOfProphets, false);
    ShouldProphetSpeak.resize(NumberOfProphets, false);
    ShouldVillagerSpeak.resize(NumberOfVillagers, false);
    Prophets.resize(NumberOfProphets);
    Villagers.resize(NumberOfVillagers);
    CommentGivenThisRound.resize(5, false);
    Guest = 0;
    ProphetsScripted = 0;
    seh_Prophet.resize(5);
    MissionHistory = MISSION_HISTORY_NONE;
    MoralityChangeOnKilled = pGlobalScriptDef->FHMoralityChangeOnKilled;
    MoralityChangeOnFreed = pGlobalScriptDef->FHMoralityChangeOnFreed;
    HasDemonDoorBeenEnabled = false;
    HasScytheCutscenePlayed = false;
    KilledAllPrisoners = true;
    FreedAllPrisoners = true;
    EvilRounds = 0;
    GoodRounds = 0;
}
}