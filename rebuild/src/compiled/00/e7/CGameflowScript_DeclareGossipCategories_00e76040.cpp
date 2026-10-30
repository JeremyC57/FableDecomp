// CGameflowScript::DeclareGossipCategories() -- register the three Bowerstone mayor gossip categories (ExposedMayor, ExposedMayorNotBecomeMayor, BribedMayor) with their rumour text, the Bowerstone Posh village and the villagers faction
// Leaf: DeclareGossipCategories
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E76040, length 784 bytes (+ int3 padding; `ret`).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CGameScriptInterface slots 717
//   AddRumourCategory (+0xb34), 718 AddNewRumourToCategory (+0xb38), 721 AddGossipVillage (+0xb44), 722
//   AddGossipFactionToCategory (+0xb48) -- the slot names of the landed CV_TravellingHeroesScript::GossipSetup 0x00EE07E0.
// Data: "ExposedMayor" 0x012E9888, "TEXT_AI_GOSSIP_MAYOR_EXPOSED" 0x012E9868, "ExposedMayorNotBecomeMayor" 0x012E984C,
//   "TEXT_AI_GOSSIP_MAYOR_EXPOSED_NOT_BECOME" 0x012E9824, "BribedMayor" 0x012E9818, "TEXT_AI_GOSSIP_MAYOR_BRIBED" 0x012E97FC,
//   "VILLAGE_BOWERSTONE_POSH" 0x012C4E90, "FACTION_VILLAGERS" 0x0125EAB4.
// Layout: the game script interface pointer at +0x40 (CQuestScriptBase model of the landed quest scripts). The interface is
//   modelled locally with its virtual slots: engine/CGameScriptInterface.h is data-only.
// Shape: every argument is a string literal converted implicitly to `const CCharString&`, so VC builds all the
//   temporaries (right to left) before the first push and destroys them after the call; no EH frame (no /EHsc).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CGameflowScript {
    unsigned char          _pad00[0x40];
    CGameScriptInterface*  GameScriptInterface;             // +0x40
    void DeclareGossipCategories();
};

void CGameflowScript::DeclareGossipCategories()
{
    GameScriptInterface->AddRumourCategory("ExposedMayor");
    GameScriptInterface->AddNewRumourToCategory("ExposedMayor", "TEXT_AI_GOSSIP_MAYOR_EXPOSED");
    GameScriptInterface->AddGossipVillage("ExposedMayor", "VILLAGE_BOWERSTONE_POSH");
    GameScriptInterface->AddGossipFactionToCategory("ExposedMayor", "FACTION_VILLAGERS");

    GameScriptInterface->AddRumourCategory("ExposedMayorNotBecomeMayor");
    GameScriptInterface->AddNewRumourToCategory("ExposedMayorNotBecomeMayor", "TEXT_AI_GOSSIP_MAYOR_EXPOSED_NOT_BECOME");
    GameScriptInterface->AddGossipVillage("ExposedMayorNotBecomeMayor", "VILLAGE_BOWERSTONE_POSH");
    GameScriptInterface->AddGossipFactionToCategory("ExposedMayorNotBecomeMayor", "FACTION_VILLAGERS");

    GameScriptInterface->AddRumourCategory("BribedMayor");
    GameScriptInterface->AddNewRumourToCategory("BribedMayor", "TEXT_AI_GOSSIP_MAYOR_BRIBED");
    GameScriptInterface->AddGossipVillage("BribedMayor", "VILLAGE_BOWERSTONE_POSH");
    GameScriptInterface->AddGossipFactionToCategory("BribedMayor", "FACTION_VILLAGERS");
}