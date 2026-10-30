// CQ_TraderConflictEvilScript::Init() -- reset the evil trader-conflict quest state, start the scream-out timer, register the BarrowFields quest region and the five boasts, then arm the bandit-follower boast flags in the master data
// Leaf: ?Init@CQ_TraderConflictEvilScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF5CD0, 625 bytes (640 with int3 padding).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; the global game interface 0x0143E8F8
//   SetTimer (slot 89, +0x164) through the inline CTimer; through the script's interface (+0x40): AddQuestRegion
//   (slot 271, +0x43c) and AddBoast (slot 303, +0x4bc) -- slot names of the landed CQ_WhiteBalverineKnotholeGladeScript::Init.
// Data: the script def at 0x0143E90C, 4 bytes below Ego_r.pdb's CScriptDef offsets (TraderConflictEvilNakedBoastCost
//   PDB +0x1CC -> retail +0x1C8, TCEKeepBanditFollowerAliveCost PDB +0x24C -> +0x248). The master data (+0x44) is
//   CQ_SunnyvaleMasterData at the PDB offsets (TCEKeepBanditFollowerAlive +0x128, TCEMadeTimeLimit +0x129).
// Layout (retail = Ego_r PDB -0x18, after the 12-byte retail AllCreatures vector at +0x48): CounterID +0x54,
//   NumberSpawned +0x58, InitialNumberInRegion +0x5C, QuestStartScreened +0x60, NextTimeToSpawnGuards +0x64,
//   MissionSucceeded +0x68, HeroAttackedBandit +0x69, PlayerEngaged +0x6A, ScreamOutTimer +0x6C.
// Shape: the zero is promoted into ebx and reused for the boasts' `false` / 0 pushes; PlayerEngaged is cleared after
//   the timer call (it is scheduled into the first CCharString build); every literal is passed straight to a
//   `const CCharString&` parameter, so the temporaries are built right to left before the pushes. The timer index is
//   read through `const CTimer* const& self = this;` (the 00703590 view): a plain member read is hoisted above the
//   member stores, retail reads it after the interface global.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* Data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CTimer { public:                                         // a script timer: the interface's timer index
    long Index;
    void SetTime(long seconds) { const CTimer* const& self = this; g_pGameScriptInterface->SetTimer(self->Index, seconds); }
};
class CScriptDef { public:
    unsigned char _pad0[0x1c8];
    long TraderConflictEvilNakedBoastCost;              // +0x1C8
    long TraderConflictEvilNakedBoastReward;            // +0x1CC
    long TraderConflictEvilNoDamageBoastCost;           // +0x1D0
    long TraderConflictEvilNoDamageBoastReward;         // +0x1D4
    long TraderConflictEvilNoWeaponsBoastCost;          // +0x1D8
    long TraderConflictEvilNoWeaponsBoastReward;        // +0x1DC
    unsigned char _pad1e0[0x68];
    long TCEKeepBanditFollowerAliveCost;                // +0x248
    long TCEKeepBanditFollowerAliveReward;              // +0x24C
    long TCEMadeTimeLimitCost;                          // +0x250
    long TCEMadeTimeLimitReward;                        // +0x254
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CQ_SunnyvaleMasterData {
    unsigned char _pad0[0x128];
    bool TCEKeepBanditFollowerAlive;                    // +0x128
    bool TCEMadeTimeLimit;                              // +0x129
};
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
};
namespace NScript {
class CQ_TraderConflictEvilScript : public CScriptBase {
public:
    CGameScriptInterface*   Game;                       // +0x40
    CQ_SunnyvaleMasterData* Data;                       // +0x44
    unsigned char           AllCreatures[0xc];          // +0x48 vector<CScriptThing>
    long                    CounterID;                  // +0x54
    long                    NumberSpawned;              // +0x58
    long                    InitialNumberInRegion;      // +0x5C
    bool                    QuestStartScreened;         // +0x60
    long                    NextTimeToSpawnGuards;      // +0x64
    bool                    MissionSucceeded;           // +0x68
    bool                    HeroAttackedBandit;         // +0x69
    bool                    PlayerEngaged;              // +0x6A
    CTimer                  ScreamOutTimer;             // +0x6C
    void Init();
};
} // namespace NScript
using NScript::CQ_TraderConflictEvilScript;

void NScript::CQ_TraderConflictEvilScript::Init()
{
    CounterID = 0;
    NumberSpawned = 0;
    InitialNumberInRegion = 0;
    QuestStartScreened = false;
    NextTimeToSpawnGuards = 24;
    MissionSucceeded = false;
    HeroAttackedBandit = false;
    ScreamOutTimer.SetTime(15);
    PlayerEngaged = false;
    Game->AddQuestRegion("Q_TraderConflictEvil", "BarrowFields");
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->TraderConflictEvilNakedBoastCost,
                   pGlobalScriptDef->TraderConflictEvilNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->TraderConflictEvilNoDamageBoastCost,
                   pGlobalScriptDef->TraderConflictEvilNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, pGlobalScriptDef->TraderConflictEvilNoWeaponsBoastCost,
                   pGlobalScriptDef->TraderConflictEvilNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCEBANDITALIVE", 0x24, pGlobalScriptDef->TCEKeepBanditFollowerAliveCost,
                   pGlobalScriptDef->TCEKeepBanditFollowerAliveReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCETIMELIMIT", 0x25, pGlobalScriptDef->TCEMadeTimeLimitCost,
                   pGlobalScriptDef->TCEMadeTimeLimitReward, false, "", 0);
    Data->TCEKeepBanditFollowerAlive = true;
    Data->TCEMadeTimeLimit = false;
}