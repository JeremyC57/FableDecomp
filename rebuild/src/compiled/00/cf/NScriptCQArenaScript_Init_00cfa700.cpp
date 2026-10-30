// CQ_ArenaScript::Init() -- register the four Arena quest regions, offer the four Arena boasts and reset the arena
// Leaf: ?Init@CQ_ArenaScript@NScript@@
// state (rounds, gold, creature counts, crowd, the sixteen spawn-needed flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CFA700, length 845 bytes (padded to 848).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; through the script's
// interface (+0x40): AddQuestRegion (slot 271, +0x43c) x4 and AddBoast (slot 303, +0x4bc) x4;
// CScriptBase::IsActiveThreadTerminating 0x00CB7940.
// Data: the script def 0x0143E90C: ArenaNaked/NoDamage boasts +0x70..+0x7c (Ego_r.pdb -4); the kill/spare-opponents
// boast values +0xadc..+0xae8 are TLC additions that Ego_r.pdb does not have (names descriptive).
// Layout: Ego_r.pdb - 0x14 up to Rounds (+0x98); retail's STLport vector is 12 bytes (PDB 16), so every member after it
// sits a further 4 bytes lower (ArenaState +0xa4 .. WhisperNeededForCutscene +0x115).
// Shape: literals are implicit conversion temporaries; the clears share ebx with the boasts' false/0 arguments; the
// spawn-flag loop checks for thread termination before every store (and once more after the loop).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptDef { public:
    unsigned char _pad_0x000[0x70];
    long ArenaNakedBoastCost;                          // +0x70
    long ArenaNakedBoastReward;                        // +0x74
    long ArenaNoDamageBoastCost;                       // +0x78
    long ArenaNoDamageBoastReward;                     // +0x7c
    unsigned char _pad_0x080[0xadc - 0x80];
    long ArenaKillOpponentsBoastCost;                  // +0xadc
    long ArenaSpareOpponentsBoastCost;                 // +0xae0
    long ArenaKillOpponentsBoastReward;                // +0xae4
    long ArenaSpareOpponentsBoastReward;               // +0xae8
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

namespace NScript {
class CQ_ArenaScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                        // +0x40
    void*                 Data;                        // +0x44
    unsigned char CrowdLoopTags[0x50];                 // +0x48 CCharString[4][5]
    long Rounds[3];                                   // +0x98 vector<CArenaRoundDef>
    long ArenaState;                                   // +0xa4
    long ArenaRound;                                   // +0xa8
    long ArenaRoundWave;                               // +0xac
    long GoldMultiplier;                               // +0xb0
    bool ChamLeaving;                                  // +0xb4
    long GoldTotal;                                    // +0xb8
    bool InHitCutsceneAlready;                         // +0xbc
    bool NeedBertForSpeech;                            // +0xbd
    long CellsVillage[3];                             // +0xc0 CScriptThing
    bool PlayerLeaving;                                // +0xcc
    bool PlayerWon;                                    // +0xcd
    long TotalCreatures[3];                            // +0xd0
    long ExtraCreatures;                               // +0xdc
    bool MissionSucceeded;                             // +0xe0
    bool MissionFailed;                                // +0xe1
    bool ArenaSpawnNeeded[16];                         // +0xe2
    long NewCrowdPoints;                               // +0xf4
    long NewCrowdHappiness;                            // +0xf8
    long NewCrowdBaseLevel;                            // +0xfc
    long GlobalCrowdTimer;                             // +0x100 CTimer
    bool FinalBattleCS;                                // +0x104
    long Whisper[3];                                  // +0x108 CScriptThing
    bool PauseCrowdChecker;                            // +0x114
    bool WhisperNeededForCutscene;                     // +0x115
    long StopTimeIndex;                                // +0x118
    void Init();
};
} // namespace NScript
using NScript::CQ_ArenaScript;

void NScript::CQ_ArenaScript::Init()
{
    Game->AddQuestRegion("Q_Arena", "ArenaExterior");
    Game->AddQuestRegion("Q_Arena", "ArenaHallOfHeroes");
    Game->AddQuestRegion("Q_Arena", "ArenaCells");
    Game->AddQuestRegion("Q_Arena", "Arena");
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->ArenaNakedBoastCost,
                   pGlobalScriptDef->ArenaNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->ArenaNoDamageBoastCost,
                   pGlobalScriptDef->ArenaNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLARENAOPPONENTS", 30, pGlobalScriptDef->ArenaKillOpponentsBoastCost,
                   pGlobalScriptDef->ArenaKillOpponentsBoastReward, true, "", 1);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_SPAREARENAOPPONENTS", 31, pGlobalScriptDef->ArenaSpareOpponentsBoastCost,
                   pGlobalScriptDef->ArenaSpareOpponentsBoastReward, false, "", 1);
    ChamLeaving = false;
    ArenaRound = 0;
    ArenaRoundWave = 0;
    NeedBertForSpeech = false;
    PlayerLeaving = false;
    PlayerWon = false;
    TotalCreatures[0] = 0;
    TotalCreatures[1] = 0;
    TotalCreatures[2] = 0;
    ExtraCreatures = 0;
    MissionSucceeded = false;
    MissionFailed = false;
    GoldMultiplier = 0;
    GoldTotal = 0;
    InHitCutsceneAlready = false;
    NewCrowdHappiness = 0;
    NewCrowdBaseLevel = 0;
    NewCrowdPoints = 200;
    PauseCrowdChecker = true;
    for (long i = 0; i < 16; ++i) {
        if (IsActiveThreadTerminating()) return;
        ArenaSpawnNeeded[i] = false;
    }
    if (IsActiveThreadTerminating()) return;
    FinalBattleCS = false;
    ArenaState = 0;
    WhisperNeededForCutscene = true;
}