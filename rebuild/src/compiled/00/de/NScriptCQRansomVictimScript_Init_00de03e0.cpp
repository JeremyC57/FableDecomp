// CQ_RansomVictimScript::Init() -- reset the ransom-victim quest state, register its five quest regions and five boasts, arm the victim boast flags in the master data and name the active enemy "RandomAttacker"
// Leaf: ?Init@CQ_RansomVictimScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DE03E0, 940 bytes (944 with int3 padding; vtable reference 0x012DCE7C).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 / operator=(const char*) 0x0099EFE0;
//   through the script's interface (+0x40): AddQuestRegion (slot 271, +0x43c) and AddBoast (slot 303, +0x4bc) -- slot
//   names of the landed CQ_WhiteBalverineKnotholeGladeScript::Init 0x00E13C60.
// Data: the script def at 0x0143E90C, 4 bytes below Ego_r.pdb's CScriptDef offsets (RansomVictimNakedBoastCost
//   PDB +0x1B4 -> retail +0x1B0, RansomVictimVictimNoDamageCost PDB +0x25C -> +0x258). The master data (+0x44) is
//   CQ_SunnyvaleMasterData at the PDB offsets (RansomVictimVictimNoDamage +0x12A .. RansomVictimHaveVictimKilled +0x12D).
// Layout (retail = Ego_r PDB -0x14): QuestStartScreened +0x48 .. ThanksCutsceneStarting +0x53, HitChief +0x54,
//   HitEvilBrotherAndProtection +0x58, SomeoneDealingWithBeingHit +0x5C, KidnappersKilled +0x60, ActiveEnemyName +0x64.
// Shape: the zero is promoted into ebx and reused for the boasts' `false` / 0 pushes and the master-data clears; the
//   member resets are scheduled into the first CCharString build; every literal is passed straight to a
//   `const CCharString&` parameter.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* Data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:
    unsigned char _pad0[0x1b0];
    long RansomVictimNakedBoastCost;                    // +0x1B0
    long RansomVictimNakedBoastReward;                  // +0x1B4
    long RansomVictimNoDamageBoastCost;                 // +0x1B8
    long RansomVictimNoDamageBoastReward;               // +0x1BC
    long RansomVictimNoWeaponsBoastCost;                // +0x1C0
    long RansomVictimNoWeaponsBoastReward;              // +0x1C4
    unsigned char _pad1c8[0x90];
    long RansomVictimVictimNoDamageCost;                // +0x258
    long RansomVictimVictimNoDamageReward;              // +0x25C
    long RansomVictimKillKidnapperCost;                 // +0x260
    long RansomVictimKillKidnapperReward;               // +0x264
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CQ_SunnyvaleMasterData {
    unsigned char _pad0[0x12a];
    bool RansomVictimVictimNoDamage;                    // +0x12A
    bool RansomVictimKidnappersKilled;                  // +0x12B
    bool RansomVictimSaveVictim;                        // +0x12C
    bool RansomVictimHaveVictimKilled;                  // +0x12D
};
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
};
namespace NScript {
class CQ_RansomVictimScript : public CScriptBase {
public:
    CGameScriptInterface*   Game;                       // +0x40
    CQ_SunnyvaleMasterData* Data;                       // +0x44
    bool                    QuestStartScreened;         // +0x48
    bool                    MissionSucceeded;           // +0x49
    bool                    MissionFailed;              // +0x4A
    bool                    RescuedVictim;              // +0x4B
    bool                    RansommerDied;              // +0x4C
    bool                    RansomHelperStarted;        // +0x4D
    bool                    RansomHelperSpoken;         // +0x4E
    bool                    ExtraAssistanceGiven;       // +0x4F
    bool                    RansomHelpersAttacked;      // +0x50
    bool                    ToldGuardToWaitOutside;     // +0x51
    bool                    EnteredNewRegion;           // +0x52
    bool                    ThanksCutsceneStarting;     // +0x53
    long                    HitChief;                   // +0x54
    long                    HitEvilBrotherAndProtection; // +0x58
    bool                    SomeoneDealingWithBeingHit; // +0x5C
    long                    KidnappersKilled;           // +0x60
    CCharString             ActiveEnemyName;            // +0x64
    void Init();
};
} // namespace NScript
using NScript::CQ_RansomVictimScript;

void NScript::CQ_RansomVictimScript::Init()
{
    QuestStartScreened = false;
    MissionFailed = false;
    MissionSucceeded = false;
    RescuedVictim = false;
    RansommerDied = false;
    RansomHelperSpoken = false;
    RansomHelperStarted = false;
    ExtraAssistanceGiven = false;
    RansomHelpersAttacked = false;
    EnteredNewRegion = false;
    ToldGuardToWaitOutside = false;
    ThanksCutsceneStarting = false;
    HitEvilBrotherAndProtection = 0;
    SomeoneDealingWithBeingHit = false;
    KidnappersKilled = 0;
    HitChief = 0;
    Game->AddQuestRegion("Q_RansomVictim", "KnotholeGlade");
    Game->AddQuestRegion("Q_RansomVictim", "Witchwood4");
    Game->AddQuestRegion("Q_RansomVictim", "Witchwood3");
    Game->AddQuestRegion("Q_RansomVictim", "Witchwood2");
    Game->AddQuestRegion("Q_RansomVictim", "WitchwoodCavern");
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->RansomVictimNakedBoastCost,
                   pGlobalScriptDef->RansomVictimNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->RansomVictimNoDamageBoastCost,
                   pGlobalScriptDef->RansomVictimNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, pGlobalScriptDef->RansomVictimNoWeaponsBoastCost,
                   pGlobalScriptDef->RansomVictimNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_RVNODAMAGE", 0x26, pGlobalScriptDef->RansomVictimVictimNoDamageCost,
                   pGlobalScriptDef->RansomVictimVictimNoDamageReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_RVKILLKIDNAPPERS", 0x27, pGlobalScriptDef->RansomVictimKillKidnapperCost,
                   pGlobalScriptDef->RansomVictimKillKidnapperReward, false, "", 0);
    Data->RansomVictimVictimNoDamage = true;
    Data->RansomVictimSaveVictim = false;
    Data->RansomVictimHaveVictimKilled = false;
    Data->RansomVictimKidnappersKilled = false;
    ActiveEnemyName = "RandomAttacker";
}