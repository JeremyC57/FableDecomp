// CQ_OrchardFarmRaidGoodScript::Init() -- register the good orchard-farm raid's five boasts (no armour, no damage, no weapons, no crates stolen, protect the guards) and activate Q_OrchardFarmRaid
// Leaf: ?Init@CQ_OrchardFarmRaidGoodScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD23A0, 534 bytes (544 with int3 padding; vtable reference 0x012DB81C).
//   Catalog label "CQ_OrchardFarmRaidEvilScript::Init" is propagated from the byte-shaped twin 0x00DD2020: this copy
//   reads the OFGood boast block and registers the NOCRATESSTOLEN / PROTECTGUARDS boasts.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; through the script's interface (+0x40):
//   AddBoast (slot 303, +0x4bc) and ActivateQuest (slot 276, +0x450), the slot names of the landed
//   CQ_WhiteBalverineKnotholeGladeScript::Init 0x00E13C60.
// Data: the script def at 0x0143E90C. Its OFGood boast block sits 4 bytes below Ego_r.pdb's CScriptDef offsets
//   (OFGoodNakedBoastCost PDB +0x184 -> retail +0x180); the two OFG_ boasts (PDB +0xE38/+0xE40) are at retail
//   +0xD88/+0xD90.
// Shape: every string literal is passed straight to a `const CCharString&` parameter (the conversion temporaries are
//   built right to left, "" first, before any push); the def's cost/reward are read inline in the argument list.
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
    unsigned char _pad0[0x180];
    long OFGoodNakedBoastCost;   // +0x180
    long OFGoodNakedBoastReward;   // +0x184
    long OFGoodNoDamageBoastCost;   // +0x188
    long OFGoodNoDamageBoastReward;   // +0x18C
    long OFGoodNoWeaponsBoastCost;   // +0x190
    long OFGoodNoWeaponsBoastReward;   // +0x194
    unsigned char _pad198[0xbf0];
    long OFG_NoCratesStolenBoastCost;   // +0xD88
    long OFG_NoCratesStolenBoastReward;   // +0xD8C
    long OFG_NoGuardsDieBoastCost;   // +0xD90
    long OFG_NoGuardsDieBoastReward;   // +0xD94
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_OrchardFarmRaidGoodScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void Init();
};
} // namespace NScript
using NScript::CQ_OrchardFarmRaidGoodScript;

void NScript::CQ_OrchardFarmRaidGoodScript::Init()
{
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->OFGoodNakedBoastCost,
                   pGlobalScriptDef->OFGoodNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->OFGoodNoDamageBoastCost,
                   pGlobalScriptDef->OFGoodNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, pGlobalScriptDef->OFGoodNoWeaponsBoastCost,
                   pGlobalScriptDef->OFGoodNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOCRATESSTOLEN", 17, pGlobalScriptDef->OFG_NoCratesStolenBoastCost,
                   pGlobalScriptDef->OFG_NoCratesStolenBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_PROTECTGUARDS", 19, pGlobalScriptDef->OFG_NoGuardsDieBoastCost,
                   pGlobalScriptDef->OFG_NoGuardsDieBoastReward, false, "", 0);
    Game->ActivateQuest("Q_OrchardFarmRaid");
}