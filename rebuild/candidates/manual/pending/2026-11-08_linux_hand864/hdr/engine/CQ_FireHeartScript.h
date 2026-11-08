#ifndef FABLE_ENGINE_CQ_FIREHEARTSCRIPT_H
#define FABLE_ENGINE_CQ_FIREHEARTSCRIPT_H
// Retail opt-in (chunk 798): NScript::CQ_FireHeartScript, new header (no includer before chunk 798; the 64 landed FireHeart rows
// keep their local models).
// Layout: Ego_r.pdb CQ_FireHeartScript (struct_layouts_egor.tsv, 520 bytes) at retail offsets: PDB - 0x14 up to CutsceneFlags
// (retail CLevelScriptBase is 0x48), then a further - 4 after each STLport map / vector (retail 12 bytes, Ego_r Dinkumware 16;
// Ego_r's vector<bool> is 20, retail's 12): 0x1A4 bytes. Checked against retail: the landed ctor 0x00D34470 (two 12-byte map
// headers +0x4c / +0x58, CScriptThing +0xa8, CCharString +0xb4 / +0xb8, the vectors, the handle vector +0x148, the three
// scripted-thing handles +0x154 / +0x164 / +0x174, CCharString +0x188, CScriptThing +0x18c; factory new 0x1A4 at 0x00D3AFC2);
// Main 0x00D35090 reads QuestStartScreenShown +0x48, MissionSucceeded +0x49, MissionFailed +0x4a, ProphetsScripted +0x144,
// KilledAllPrisoners +0x1a2, FreedAllPrisoners +0x1a3 and assigns VillagerEscapeMarker +0x18c; PlayCutscene 0x00D39330 reads
// CutsceneFlags +0x4c, csargs +0x58, Guest +0x140, seh_Prophet +0x148, seh_BriarRose +0x154, seh_Scythe +0x164, seh_GuildMaster +0x174.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (Game +0x40, MasterScriptData +0x44).
// Contract: the includer defines FABLE_RETAIL_NSCRIPT and FABLE_RETAIL_BASE_OBJECT and includes stlport/fable_containers.h and
// stlport/fable_map.h before this header (the CScriptGameResourceObjectScriptedThingBase header needs CCountedPointer first).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_FireHeartScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
class CPersistContext;
class CFireHeartPatternDef;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_FireHeartScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum EMissionHistory { };                                            // Ego_r.pdb type name; enumerators not recovered
    CQ_FireHeartScript(CScriptDataBase*, CGameScriptInterfaceBase&);     // 0x00D34470
    virtual ~CQ_FireHeartScript();                                       // 0x00D3B010
    virtual void RegisterMain();                                         // 0x00D34660
    virtual void Main();                                                 // 0x00D35090
    virtual void Init();                                                 // 0x00D347B0
    virtual void OnPersist(CPersistContext&);                            // 0x00D34720
    void CheckGameEnding();                                              // 0x00D360E0
    void SetupPuzzleBlocks();                                            // 0x00D36140
    void RestoreFollowers();                                             // 0x00D361F0
    void RunThemeOverriding();                                           // 0x00D35ED0
    void RunVillagerComments();                                          // 0x00D38D20
    void PlayCutscene(CCharString);                                      // 0x00D39330
    void StartRound();                                                   // 0x00D39960
    void DisableFollowers();                                             // 0x00D3A050
#ifdef FABLE_RETAIL_CQ_FIREHEARTSCRIPT_PRISONER
    // Retail opt-in (chunk 837): the two members CFH_Prisoner::Main 0x00D3A4A0 calls on its parent. Ego_r.pdb
    // ?UpdateHistory@CQ_FireHeartScript@NScript@@QAEX_N@Z (landed row 0x00D3A920, called with push 0 / push 1) and
    // ?CheckCompletion@CQ_FireHeartScript@NScript@@QAEXXZ (landed row 0x00D3AD60, the CSpawnedFunc member pointer of the
    // "ParentClass." + "CheckCompletion" thread).
    void UpdateHistory(bool);                                            // 0x00D3A920
    void CheckCompletion();                                              // 0x00D3AD60
#endif
    bool QuestStartScreenShown;                                          // +0x48
    bool MissionSucceeded;                                               // +0x49
    bool MissionFailed;                                                  // +0x4a
    std::map<CCharString, bool> CutsceneFlags;                           // +0x4c
    std::map<CCharString, CCharString> csargs;                           // +0x58
    long FreedProphets;                                                  // +0x64
    long KilledProphets;                                                 // +0x68
    long StartNumberOfSuns;                                              // +0x6c
    long NumberOfRounds;                                                 // +0x70
    long NumberOfPatterns;                                               // +0x74
    long NumberOfProphets;                                               // +0x78
    long NumberOfPads;                                                   // +0x7c
    long NumberOfSuns;                                                   // +0x80
    long NumberOfVillagers;                                              // +0x84
    long PuzzleRound;                                                    // +0x88
    bool ResetBlocks;                                                    // +0x8c
    long NumBlocksReset;                                                 // +0x90
    bool RoundEnded;                                                     // +0x94
    bool RoundBeginning;                                                 // +0x95
    bool PreventRoundEnd;                                                // +0x96
    bool HasHeroHitVillagersGroupOne;                                    // +0x97
    bool HasHeroHitVillagersGroupTwo;                                    // +0x98
    bool InfoDisplayed;                                                  // +0x99
    long EvilRounds;                                                     // +0x9c
    long GoodRounds;                                                     // +0xa0
    float PadRadius;                                                     // +0xa4
    CScriptThing Effect;                                                 // +0xa8
    CCharString PadSunFX;                                                // +0xb4
    CCharString PadMoonFX;                                               // +0xb8
    std::vector<bool> StartingPattern;                                   // +0xbc
    std::vector<bool> ProphetsKilled;                                    // +0xc8
    std::vector<bool> ProphetsReleased;                                  // +0xd4
    std::vector<bool> ShouldProphetSpeak;                                // +0xe0
    std::vector<bool> ShouldVillagerSpeak;                               // +0xec
    std::vector<bool> CommentGivenThisRound;                             // +0xf8
    std::vector<CScriptThing> Prophets;                                  // +0x104
    std::vector<CScriptThing> Villagers;                                 // +0x110
    std::vector<CScriptThing> Followers;                                 // +0x11c
    std::vector<CScriptGameResourceObjectScriptedThingBase> FollowerHandles;   // +0x128
    std::vector<CFireHeartPatternDef> Pattern;                           // +0x134
    long Guest;                                                          // +0x140
    long ProphetsScripted;                                               // +0x144
    std::vector<CScriptGameResourceObjectScriptedThingBase> seh_Prophet; // +0x148
    CScriptGameResourceObjectScriptedThingBase seh_BriarRose;            // +0x154
    CScriptGameResourceObjectScriptedThingBase seh_Scythe;               // +0x164
    CScriptGameResourceObjectScriptedThingBase seh_GuildMaster;          // +0x174
    EMissionHistory MissionHistory;                                      // +0x184
    CCharString ProphetComment;                                          // +0x188
    CScriptThing VillagerEscapeMarker;                                   // +0x18c
    float MoralityChangeOnKilled;                                        // +0x198
    float MoralityChangeOnFreed;                                         // +0x19c
    bool HasDemonDoorBeenEnabled;                                        // +0x1a0
    bool HasScytheCutscenePlayed;                                        // +0x1a1
    bool KilledAllPrisoners;                                             // +0x1a2
    bool FreedAllPrisoners;                                              // +0x1a3
    class CDF_Fireheart;                                                 // Alloc 0x00D362B0
    class CPuzzleBlockMoon;                                              // Alloc 0x00D363A0
    class CPuzzleBlockSun;                                               // Alloc 0x00D36470
    class CFH_Prisoner;                                                  // Alloc 0x00D36540
    class CProphetCage;                                                  // Alloc 0x00D36680
    class CFH_GuildMaster;                                               // Alloc 0x00D36790
    class CFH_Scythe;                                                    // Alloc 0x00D37140
    class CFH_BriarRose;                                                 // Alloc 0x00D37240
    class CFH_Villager;                                                  // Alloc 0x00D373D0
    class CFH_MemorialCutsceneMarker;                                    // Alloc 0x00D37C30
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_FireHeartScript) == 0x1A4);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_FIREHEARTSCRIPT_H
