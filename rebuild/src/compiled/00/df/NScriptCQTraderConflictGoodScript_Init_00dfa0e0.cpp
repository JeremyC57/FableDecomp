// CQ_TraderConflictGoodScript::Init() -- reset the good trader-conflict quest state, register its two bandit-camp quest regions and five boasts, then arm the kill-no-bandits boast flags in the master data
// Leaf: ?Init@CQ_TraderConflictGoodScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DFA0E0, 681 bytes (688 with int3 padding).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; through the script's interface (+0x40):
//   AddQuestRegion (slot 271, +0x43c) and AddBoast (slot 303, +0x4bc) -- slot names of the landed
//   CQ_WhiteBalverineKnotholeGladeScript::Init 0x00E13C60.
// Data: the script def at 0x0143E90C, 4 bytes below Ego_r.pdb's CScriptDef offsets (TraderConflictGoodNakedBoastCost
//   PDB +0x1E4 -> retail +0x1E0, TCGKillNoBanditsCost PDB +0x23C -> +0x238). The master data (+0x44) is
//   CQ_SunnyvaleMasterData at the PDB offsets (TCGKillNoBandits +0x126, TCGMadeTimeLimit +0x127).
// Layout (retail = Ego_r PDB -0x18, after the 12-byte retail AllCreatures vector at +0x48): TradersFollowing +0x54,
//   TradersReachedTeleporter +0x58, BanditSecurityLinesSaid +0x5C, FirstRegionDead +0x60 .. MissionSucceeded +0x67
//   (the landed WatchTimeLimit 0x00DFAEB0 has MissionSucceeded +0x67).
// Shape: the zero is promoted into ebx and reused for the boasts' `false` / 0 pushes; the member resets are scheduled
//   into the first CCharString build; every literal is passed straight to a `const CCharString&` parameter.
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
class CScriptDef { public:
    unsigned char _pad0[0x1e0];
    long TraderConflictGoodNakedBoastCost;              // +0x1E0
    long TraderConflictGoodNakedBoastReward;            // +0x1E4
    long TraderConflictGoodNoDamageBoastCost;           // +0x1E8
    long TraderConflictGoodNoDamageBoastReward;         // +0x1EC
    long TraderConflictGoodNoWeaponsBoastCost;          // +0x1F0
    long TraderConflictGoodNoWeaponsBoastReward;        // +0x1F4
    unsigned char _pad1f8[0x40];
    long TCGKillNoBanditsCost;                          // +0x238
    long TCGKillNoBanditsReward;                        // +0x23C
    long TCGMadeTimeLimitCost;                          // +0x240
    long TCGMadeTimeLimitReward;                        // +0x244
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CQ_SunnyvaleMasterData {
    unsigned char _pad0[0x126];
    bool TCGKillNoBandits;                              // +0x126
    bool TCGMadeTimeLimit;                              // +0x127
};
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
};
namespace NScript {
class CQ_TraderConflictGoodScript : public CScriptBase {
public:
    CGameScriptInterface*   Game;                       // +0x40
    CQ_SunnyvaleMasterData* Data;                       // +0x44
    unsigned char           AllCreatures[0xc];          // +0x48 vector<CScriptThing>
    long                    TradersFollowing;           // +0x54
    long                    TradersReachedTeleporter;   // +0x58
    long                    BanditSecurityLinesSaid;    // +0x5C
    bool                    FirstRegionDead;            // +0x60
    bool                    EnteredNewRegion;           // +0x61
    bool                    IntroDone;                  // +0x62
    bool                    OutroStart;                 // +0x63
    bool                    OutroDone;                  // +0x64
    bool                    OpenedCage;                 // +0x65
    bool                    CommentedOnBanditCostume;   // +0x66
    bool                    MissionSucceeded;           // +0x67
    void Init();
};
} // namespace NScript
using NScript::CQ_TraderConflictGoodScript;

void NScript::CQ_TraderConflictGoodScript::Init()
{
    TradersFollowing = 0;
    TradersReachedTeleporter = 0;
    BanditSecurityLinesSaid = 0;
    IntroDone = false;
    OutroStart = false;
    OutroDone = false;
    OpenedCage = false;
    FirstRegionDead = false;
    EnteredNewRegion = false;
    CommentedOnBanditCostume = false;
    MissionSucceeded = false;
    Game->AddQuestRegion("Q_TraderConflictGood", "BanditCampEntrance");
    Game->AddQuestRegion("Q_TraderConflictGood", "BanditCampCentre");
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->TraderConflictGoodNakedBoastCost,
                   pGlobalScriptDef->TraderConflictGoodNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->TraderConflictGoodNoDamageBoastCost,
                   pGlobalScriptDef->TraderConflictGoodNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, pGlobalScriptDef->TraderConflictGoodNoWeaponsBoastCost,
                   pGlobalScriptDef->TraderConflictGoodNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCGKILLNOONE", 0x22, pGlobalScriptDef->TCGKillNoBanditsCost,
                   pGlobalScriptDef->TCGKillNoBanditsReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCGTIMELIMIT", 0x23, pGlobalScriptDef->TCGMadeTimeLimitCost,
                   pGlobalScriptDef->TCGMadeTimeLimitReward, false, "", 0);
    Data->TCGKillNoBandits = true;
    Data->TCGMadeTimeLimit = false;
}