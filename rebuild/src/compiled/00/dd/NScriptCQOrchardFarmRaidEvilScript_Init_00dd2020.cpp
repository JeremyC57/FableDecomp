// NScript::CQ_OrchardFarmRaidEvilScript::Init() -- register the evil orchard-farm raid's five boasts (no armour, no damage, no weapons, no health potions, protect the bandits) and activate Q_OrchardFarmRaid
// Leaf: ?Init@CQ_OrchardFarmRaidEvilScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD2020, 534 bytes (544 with int3 padding; vtable reference 0x012DB7BC).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; through the script's interface (+0x40):
//   AddBoast (slot 303, +0x4bc) and ActivateQuest (slot 276, +0x450), the slot names of the landed
//   CQ_WhiteBalverineKnotholeGladeScript::Init 0x00E13C60.
// Data: the script def at 0x0143E90C. Its OFEvil boast block sits 4 bytes below Ego_r.pdb's CScriptDef offsets
//   (OFEvilNakedBoastCost PDB +0x16C -> retail +0x168); the two OFE_ boasts (PDB +0xE28/+0xE30) are at retail
//   +0xD78/+0xD80 (the retail def lacks debug members between).
// Shape: every string literal is passed straight to a `const CCharString&` parameter (the conversion temporaries are
//   built right to left, "" first, before any push); the def's cost/reward are read inline in the argument list.
// Naming wave 35: identity CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00dd2300 at 00cd7020 with name "Q_OrchardFarmRaidEvil"; Alloc news + stores script vtable 012db7b0; PDB ?Alloc@CQ_OrchardFarmRaidEvilScript@NScript@@SIPAVCScriptBase@@; script vtable slot 3 (slot 1 pushes "Main" 0x012C2F94); PDB Init; refs=1; was CQ_OrchardFarmRaidEvilScript::Init (namespace spelling) (wave3/labels_alloc.tsv); PDB ?Init@CQ_OrchardFarmRaidEvilScript@NScript@@UAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Init@CQ_OrchardFarmRaidEvilScript@@UAEXXZ`; this source now defines the PDB member (class model unchanged); same bytes.
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
    unsigned char _pad0[0x168];
    long OFEvilNakedBoastCost;   // +0x168
    long OFEvilNakedBoastReward;   // +0x16C
    long OFEvilNoDamageBoastCost;   // +0x170
    long OFEvilNoDamageBoastReward;   // +0x174
    long OFEvilNoWeaponsBoastCost;   // +0x178
    long OFEvilNoWeaponsBoastReward;   // +0x17C
    unsigned char _pad180[0xbf8];
    long OFE_NoHealthPotionBoastCost;   // +0xD78
    long OFE_NoHealthPotionBoastReward;   // +0xD7C
    long OFE_NoBanditsDieBoastCost;   // +0xD80
    long OFE_NoBanditsDieBoastReward;   // +0xD84
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_OrchardFarmRaidEvilScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
public:
    virtual void Init();                          // PDB ?Init@CQ_OrchardFarmRaidEvilScript@NScript@@UAEXXZ
};
}
using NScript::CQ_OrchardFarmRaidEvilScript;

void NScript::CQ_OrchardFarmRaidEvilScript::Init()
{
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->OFEvilNakedBoastCost,
                   pGlobalScriptDef->OFEvilNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->OFEvilNoDamageBoastCost,
                   pGlobalScriptDef->OFEvilNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, pGlobalScriptDef->OFEvilNoWeaponsBoastCost,
                   pGlobalScriptDef->OFEvilNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_USENOHEALTHPOTIONS", 8, pGlobalScriptDef->OFE_NoHealthPotionBoastCost,
                   pGlobalScriptDef->OFE_NoHealthPotionBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_PROTECTBANDITS", 18, pGlobalScriptDef->OFE_NoBanditsDieBoastCost,
                   pGlobalScriptDef->OFE_NoBanditsDieBoastReward, true, "", 0);
    Game->ActivateQuest("Q_OrchardFarmRaid");
}