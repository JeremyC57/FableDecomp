// CQ_MinionCampScript::Init() -- reset the mission state and both timers, copy the minion spawn periods from the script def, register the quest region and the quest's four boasts
// Leaf: ?Init@CQ_MinionCampScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D9E2E0, length 571 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; through the interface at +0x40:
//   AddQuestRegion (slot 271, +0x43c), AddBoast (slot 303, +0x4bc); the timer service (global 0x0143E8F8) SetTimer
//   (slot 89, +0x164) through an inline CTimer::Set.
// Data: the script def 0x0143E90C (retail = Ego_r.pdb CScriptDef - 4 - 4 per earlier STLport vector): MinionCamp boast
//   cost/reward pairs +0x150..+0x164, MinionCampBriarNoDamageCost/Reward +0x278/+0x27c, MCMin/MaxMinionSpawnPeriod
//   +0xd3c/+0xd40.
// Layout: the landed CQ_MinionCampScript ctor 0x00D9E210 (retail = Ego_r.pdb - 0x14, the two STLport lists 4 bytes).
// Shape: as CQ_HangingTreeEvilScript::Init 0x00D68FF0 (literals straight to `const CCharString&` parameters; the zero
//   hoisted into ebx); the def pointer is re-read after the float store.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CTimer { public:                                         // a script timer: the interface's timer index
    long Index;
    void Set(long time) { const long& index = Index; g_pGameScriptInterface->SetTimer(index, time); }
};
class CScriptDef { public:
    unsigned char _pad0[0x150];
    long MinionCampNakedBoastCost;                      // +0x150
    long MinionCampNakedBoastReward;                    // +0x154
    long MinionCampNoDamageBoastCost;                   // +0x158
    long MinionCampNoDamageBoastReward;                 // +0x15c
    long MinionCampNoWeaponsBoastCost;                  // +0x160
    long MinionCampNoWeaponsBoastReward;                // +0x164
    unsigned char _pad168[0x278 - 0x168];
    long MinionCampBriarNoDamageCost;                   // +0x278
    long MinionCampBriarNoDamageReward;                 // +0x27c
    unsigned char _pad280[0xd3c - 0x280];
    float MCMinMinionSpawnPeriod;                       // +0xd3c
    float MCMaxMinionSpawnPeriod;                       // +0xd40
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
};
namespace NScript {
class CQ_MinionCampScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
    bool QuestStartScreenShown;                         // +0x48
    bool IntroDone;                                     // +0x49
    bool BriarRoseAtAltar;                              // +0x4a
    bool MinionsSummoned;                               // +0x4b
    bool PortalSealed;                                  // +0x4c
    bool MissionFailed;                                 // +0x4d
    long NumberOfMinions;                               // +0x50
    long MaxNumberOfMinions;                            // +0x54
    float MinMinionSpawnPeriod;                         // +0x58
    float MaxMinionSpawnPeriod;                         // +0x5c
    CTimer ChantTimer;                                  // +0x60
    long ChantTimerBar;                                 // +0x64
    bool ChantTimerPaused;                              // +0x68
    long PausedTime;                                    // +0x6c
    CTimer MinionSpawnTimer;                            // +0x70
    long CurrentStage;                                  // +0x74 (MissionStage)
    unsigned char HeroAttackerOne[0xc];                 // +0x78 CScriptThing
    unsigned char HeroAttackerTwo[0xc];                 // +0x84 CScriptThing
    void* LeftSpawnPoints;                              // +0x90 STLport list (node pointer)
    void* RightSpawnPoints;                             // +0x94 STLport list (node pointer)
    bool ShouldSpawn;                                   // +0x98
    unsigned char SpawnPointToSpawn[0xc];               // +0x9c CScriptThing
    long BriarState;                                    // +0xa8 (BriarRoseState)
    void Init();
};
} // namespace NScript
using NScript::CQ_MinionCampScript;

void NScript::CQ_MinionCampScript::Init()
{
    QuestStartScreenShown = false;
    IntroDone = false;
    BriarRoseAtAltar = false;
    MinionsSummoned = false;
    PortalSealed = false;
    MissionFailed = false;
    NumberOfMinions = 0;
    ChantTimer.Set(0);
    ChantTimerPaused = false;
    MinionSpawnTimer.Set(0);
    MaxNumberOfMinions = 3;
    MinMinionSpawnPeriod = pGlobalScriptDef->MCMinMinionSpawnPeriod;
    MaxMinionSpawnPeriod = pGlobalScriptDef->MCMaxMinionSpawnPeriod;
    CurrentStage = 0;
    BriarState = 0;
    Game->AddQuestRegion("Q_MinionCamp", "Darkwood3");
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->MinionCampNakedBoastCost,
                   pGlobalScriptDef->MinionCampNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->MinionCampNoDamageBoastCost,
                   pGlobalScriptDef->MinionCampNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, pGlobalScriptDef->MinionCampNoWeaponsBoastCost,
                   pGlobalScriptDef->MinionCampNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_BRIARNODAMAGE", 0x2a, pGlobalScriptDef->MinionCampBriarNoDamageCost,
                   pGlobalScriptDef->MinionCampBriarNoDamageReward, false, "", 0);
}