// CQ_HobbeCaveScript::Init() -- register the quest's eleven regions, reset the mission state (the timer from the script def), flag the master data and register the six boasts
// Leaf: ?Init@CQ_HobbeCaveScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D8D640, 1472 bytes (int3 padded).
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; CGameScriptInterface AddQuestRegion (slot 271,
//   +0x43c), AddBoast (slot 303, +0x4bc); script def pGlobalScriptDef 0x0143E90C (boast cost/reward pairs +0xd8.. and +0xcf0..,
//   HobbeCave timer +0xccc).
// Layout: Game +0x40, PMasterData +0x44 (+0x10d / +0x110 / +0x114), bools +0x48..+0x50, longs +0x58/+0x5c, bool +0x60,
//   timer +0x64, bools +0x68..+0x6c, longs +0x70/+0x74 (descriptive names: the PDB does not cover the quest scripts).
// Shape: landed CQ_HangingTreeGoodScript::Init 0x00D6F020 (literals straight to the `const CCharString&` parameters).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"                              // CCharString (agentNF60: was a local stand-in)
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// agentNF60: the local CScriptDef model named these members descriptively; engine/CScriptDef.h (Ego_r / FableWin names at the
//   retail offsets) names them: NakedBoastCost -> HobbeCaveNakedBoastCost (+0xd8), NakedBoastReward -> HobbeCaveNakedBoastReward (+0xdc), NoDamageBoastCost -> HobbeCaveNoDamageBoastCost (+0xe0), NoDamageBoastReward -> HobbeCaveNoDamageBoastReward (+0xe4), NoWeaponsBoastCost -> HobbeCaveNoWeaponsBoastCost (+0xe8), NoWeaponsBoastReward -> HobbeCaveNoWeaponsBoastReward (+0xec), HobbeCaveTimer -> HC_HobbeSpawnCount (+0xccc), KillNymphBoastCost -> HC_KillNymphBoastCost (+0xcf0), KillNymphBoastReward -> HC_KillNymphBoastReward (+0xcf4), SacrificeBoastCost -> HC_SacrificeBoastCost (+0xcf8), SacrificeBoastReward -> HC_SacrificeBoastReward (+0xcfc), BoyNoHarmBoastCost -> HC_BoyNoHarmBoastCost (+0xd00), BoyNoHarmBoastReward -> HC_BoyNoHarmBoastReward (+0xd04).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>; agentNF60: was a local CScriptDef model and `CScriptDef* pGlobalScriptDef`)
struct CHobbeCaveMasterData {
    unsigned char _pad[0x10d];
    bool HobbeCaveDone;                                 // +0x10d
    unsigned char _pad10e[2];
    long HobbeCaveState;                                // +0x110
    bool HobbeCaveActive;                               // +0x114
};
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_HobbeCaveScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    CHobbeCaveMasterData* PMasterData;                  // +0x44
    bool Flag48, Flag49, Flag4A, Flag4B, Flag4C, Flag4D, Flag4E, Flag4F, Flag50;   // +0x48..+0x50
    unsigned char _pad51[7];
    long Count58;                                       // +0x58
    long Count5C;                                       // +0x5c
    bool Flag60;                                        // +0x60
    unsigned char _pad61[3];
    long Timer;                                         // +0x64
    bool Flag68, Flag69, Flag6A, Flag6B, Flag6C;        // +0x68..+0x6c
    unsigned char _pad6d[3];
    long Count70;                                       // +0x70
    long Count74;                                       // +0x74
    void Init();
};
} // namespace NScript
using NScript::CQ_HobbeCaveScript;

void NScript::CQ_HobbeCaveScript::Init()
{
    Game->AddQuestRegion("Q_HobbeCave", "GrannysHouse");
    Game->AddQuestRegion("Q_HobbeCave", "Greatwood");
    Game->AddQuestRegion("Q_HobbeCave", "GreatwoodCaves");
    Game->AddQuestRegion("Q_HobbeCave", "DemonDoor_GreatwoodCaves");
    Game->AddQuestRegion("Q_HobbeCave", "HobbeCaveEntranceTunnel");
    Game->AddQuestRegion("Q_HobbeCave", "HobbeCaveSideRoom");
    Game->AddQuestRegion("Q_HobbeCave", "HobbeCaveLarder");
    Game->AddQuestRegion("Q_HobbeCave", "HobbeCaveNymphChamber");
    Game->AddQuestRegion("Q_HobbeCave", "HobbeCaveMainChamber");
    Game->AddQuestRegion("Q_HobbeCave", "HobbeCaveNymphFocal");
    Game->AddQuestRegion("Q_HobbeCave", "DemonDoor_GreatwoodGrannysHouse");
    Flag48 = false;
    Flag49 = false;
    Flag4A = false;
    Flag4B = false;
    Flag4C = false;
    Flag4D = false;
    Flag4E = false;
    Flag4F = false;
    Flag50 = false;
    Count58 = 0;
    Count5C = 0;
    Flag60 = false;
    Timer = pGlobalScriptDef->HC_HobbeSpawnCount;
    Flag68 = false;
    Flag69 = false;
    Flag6A = false;
    Flag6B = false;
    Flag6C = false;
    Count70 = 0;
    Count74 = 0;
    PMasterData->HobbeCaveDone = false;
    PMasterData->HobbeCaveState = 1;
    PMasterData->HobbeCaveActive = true;
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->HobbeCaveNakedBoastCost, pGlobalScriptDef->HobbeCaveNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->HobbeCaveNoDamageBoastCost, pGlobalScriptDef->HobbeCaveNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, pGlobalScriptDef->HobbeCaveNoWeaponsBoastCost, pGlobalScriptDef->HobbeCaveNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLNYMPH", 0x15, pGlobalScriptDef->HC_KillNymphBoastCost, pGlobalScriptDef->HC_KillNymphBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_SACRIFICE", 0x16, pGlobalScriptDef->HC_SacrificeBoastCost, pGlobalScriptDef->HC_SacrificeBoastReward, true, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_BOYNOHARM", 0x17, pGlobalScriptDef->HC_BoyNoHarmBoastCost, pGlobalScriptDef->HC_BoyNoHarmBoastReward, false, "", 0);
}