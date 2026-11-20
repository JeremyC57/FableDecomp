// CTCInventoryTrade::FinalRefreshSubtitles(NUISystem::CComponent* component, CSubtitleMap& subtitles) -- rewrites the one-off subtitle child of `component`: the component's own type-6 child text when the component has no entry in `subtitles`, otherwise the entry's text followed by " x N" (the localised multiplier and the count) when the count is positive
// Leaf: ?FinalRefreshSubtitles@CTCInventoryTrade@@AAEXPAVCComponent@NUISystem@@AAV?$map@PAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@4@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059D1B1, length 191 bytes (`ret 8`). Frame-pointer size TU: optimize("y",off) + ("s",on).
// A byte twin of 0x0059D761 (the PC_SUBTITLE child); the oracle labels both FinalRefreshSubtitles.
// Callees: map<CComponent*,...>::find @ 0x005B1A8D (catalog "std::set_FindLE_Duplicate"; STLport node: key +0x10, text +0x14, count
// +0x18; end() is the header at +0), CWideString::CWideString() @ 0x0099AED0, operator=(const CWideString&) @ 0x0099B7D0, operator+=
// (const CWideString&) @ 0x0099B8D0, ~CWideString @ 0x0099B510, GetXString() @ 0x005CC772 and GFIntToWideString(long) @ 0x00405F00
// (`__fastcall`, CWideString by value through the hidden pointer in ecx), CCharString(const char*, long) @ 0x0099EBF0 ("UI_ONE_OFF_SUBTITLE"
// 0x0124E72C), NUISystem::CComponent virtual slots 9 (+0x24, the typed child getter), 142 (+0x238, the child's text), 3 (+0x0C, FindChild
// by name, CCharString by value) and 144 (+0x240, SetText(const CWideString&, bool, bool)).
// Shape: the map reference is read into edi before the find and the component replaces it there afterwards; the text local lives in
// the dead map argument slot ([ebp+0Ch]); the two by-value temporaries of the count arm are homed in the dead component slot ([ebp+8])
// and destroyed right after each `+=`; the FindChild name is built in place on the stack (`push ecx ; mov ecx,esp`).
// Naming (NF168, 2026-11-20): moved onto the shared headers. Own name: Ego_r.pdb / FableWin.pdb private
// ?FinalRefreshSubtitles@CTCInventoryTrade@@AAEXPAVCComponent@NUISystem@@AAV?$map@...@std@@@Z (engine/CTCInventoryTrade.h opt-in
// FABLE_RETAIL_CTCINVENTORYTRADE_FINALSUBTITLES), the spelling the landed caller RefreshSubtitles 0x0059E108 binds; the map is the real
// STLport map<CComponent*, pair<CWideString,long> > whose _M_find 0x005B1A8D is called out of line (FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE,
// the NF50 trade-refresh form); slot 9 / 142 / 3 / 144 are FindFirstChildOfType / CText::GetValue / FindChild / CText::SetValue;
// callee 0x005CC772 is CTCInventoryStats::GetXString.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_FINALSUBTITLES
#define FABLE_RETAIL_CTCINVENTORYTRADE_BUYITEMPC
#define FABLE_RETAIL_CTCINVENTORYTRADE_BUYPOSSIBILITYPC
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_CTCSHOP_SELLPRICE
#define FABLE_RETAIL_CTCSHOP_SHOPTYPE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CTCHEROSTATS_ADDMONEY
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER   // non-const GetMainPlayer 0x00449970, as retail calls
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CTCINVENTORYITEM_ISSELLABLE   // NF34: 0x005D9B30 is IsSellable (Ego_r + FableWin; def +0x5C)
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CHEROTITLEDEF
#define FABLE_RETAIL_CHAIRCARDDEF
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHero.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CHeroTitleDef.h"
#include "engine/CTattooDef.h"
#include "engine/CHairCardDef.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_enums.h"
#include "engine/CCountedPointer.h"
using namespace NUISystem;
using NGameText::CDataBank;

CWideString FABLE_FASTCALL GFIntToWideString(long value);     // 0x00405F00

void CTCInventoryTrade::FinalRefreshSubtitles(CComponent* component, std::map<CComponent*,std::pair<CWideString,long> >& subtitles)
{
    std::map<CComponent*,std::pair<CWideString,long> >::iterator found = subtitles.find(component);
    CWideString text;
    if (found == subtitles.end()) {
        text = static_cast<CText*>(component->FindFirstChildOfType(UI_TYPE_TEXT))->GetValue();
    } else {
        text = (*found).second.first;
        if ((*found).second.second > 0) {
            text += CTCInventoryStats::GetXString();
            text += GFIntToWideString((*found).second.second);
        }
    }
    static_cast<CText*>(component->FindChild("UI_ONE_OFF_SUBTITLE"))->SetValue(text, true, true);
}
