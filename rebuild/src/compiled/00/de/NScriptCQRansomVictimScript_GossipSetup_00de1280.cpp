// CQ_RansomVictimScript::GossipSetup() -- register the two Knothole Glade gossip categories (ChiefsSonDied, ChiefsSonSurvived) with their rumour text, the Knothole Glade village and the villagers faction
// Leaf: GossipSetup
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DE1280, 519 bytes (528 with int3 padding; `ret`).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CGameScriptInterface slots 717
//   AddRumourCategory (+0xb34), 718 AddNewRumourToCategory (+0xb38), 721 AddGossipVillage (+0xb44), 722
//   AddGossipFactionToCategory (+0xb48) -- the slot names of the landed DeclareGossipCategories 0x00E76040.
// Data: "ChiefsSonDied" 0x012DD128, "TEXT_AI_GOSSIP_CHIEFS_SON_DIED" 0x012DD108, "ChiefsSonSurvived" 0x012DD0F4,
//   "TEXT_AI_GOSSIP_CHIEFS_SON_SURVIVED" 0x012DD0D0, "VILLAGE_KNOTHOLE_GLADE" 0x012C50AC, "FACTION_VILLAGERS" 0x0125EAB4.
// Layout: the game script interface pointer at +0x40 (the quest-script model of the landed scripts); the interface
//   is modelled locally with its virtual slots (engine/CGameScriptInterface.h is data-only).
// Shape: every argument is a string literal converted implicitly to `const CCharString&`, so VC builds all the
//   temporaries (right to left) before the first push and destroys them after the call.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CQ_RansomVictimScript {
public:
    unsigned char          _pad00[0x40];
    CGameScriptInterface*  GameScriptInterface;             // +0x40
    void GossipSetup();
};
} // namespace NScript
using NScript::CQ_RansomVictimScript;

void NScript::CQ_RansomVictimScript::GossipSetup()
{
    GameScriptInterface->AddRumourCategory("ChiefsSonDied");
    GameScriptInterface->AddNewRumourToCategory("ChiefsSonDied", "TEXT_AI_GOSSIP_CHIEFS_SON_DIED");
    GameScriptInterface->AddGossipVillage("ChiefsSonDied", "VILLAGE_KNOTHOLE_GLADE");
    GameScriptInterface->AddGossipFactionToCategory("ChiefsSonDied", "FACTION_VILLAGERS");

    GameScriptInterface->AddRumourCategory("ChiefsSonSurvived");
    GameScriptInterface->AddNewRumourToCategory("ChiefsSonSurvived", "TEXT_AI_GOSSIP_CHIEFS_SON_SURVIVED");
    GameScriptInterface->AddGossipVillage("ChiefsSonSurvived", "VILLAGE_KNOTHOLE_GLADE");
    GameScriptInterface->AddGossipFactionToCategory("ChiefsSonSurvived", "FACTION_VILLAGERS");
}