// CV_BookCollectingScript::AddGossip(CCharString category, CCharString rumour, CCharString village, CCharString faction) -- registers a gossip category with the game: the category, one rumour in it, the village and the faction it belongs to (all four strings by value)
// Leaf: AddGossip
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E55C60, length 122 bytes (padded to 128), `ret 10h`.
// Callees: through the quest's game script interface (+0x40, vtable 0x01260F0C): AddRumourCategory (slot 717,
// +0xb34, 0x008A9DB0), AddNewRumourToCategory (slot 718, +0xb38), AddGossipVillage (slot 721, +0xb44),
// AddGossipFactionToCategory (slot 722, +0xb48); ~CCharString @ 0x0099EAE0 (the four by-value parameters, destroyed by
// the callee in order).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);
    ~CCharString();                                // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CV_BookCollectingScript {
public:
    unsigned char _base[0x40];                     // CScriptBase
    CGameScriptInterface* Game;                    // +0x40
    void* Data;                                    // +0x44
    void AddGossip(CCharString category, CCharString rumour, CCharString village, CCharString faction);
};
} // namespace NScript
using NScript::CV_BookCollectingScript;
void NScript::CV_BookCollectingScript::AddGossip(CCharString category, CCharString rumour, CCharString village, CCharString faction)
{
    Game->AddRumourCategory(category);
    Game->AddNewRumourToCategory(category, rumour);
    Game->AddGossipVillage(category, village);
    Game->AddGossipFactionToCategory(category, faction);
}