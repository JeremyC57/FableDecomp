// CTCInventoryTrade::FinalConstructSubtitlesPC(NUISystem::CComponent* component, CSubtitleMap& subtitles, NUISystem::CComponent* list) -- PC layout: build a PC_SUBTITLE text component for `component` (its own type-6 child text when it has no entry in `subtitles`, otherwise the entry's text plus " x N" for a positive count), add it under `component` and register it with `list`
// Leaf: ?FinalConstructSubtitlesPC@CTCInventoryTrade@@AAEXPAVCComponent@NUISystem@@AAV?$map@PAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@4@@std@@0@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059D65A, length 263 bytes, `ret 0Ch`. Frame-pointer size TU: optimize("y",off) +
// ("s",on). FableWin.pdb ?FinalConstructSubtitlesPC@CTCInventoryTrade@@AAEXPAVCComponent@NUISystem@@AAV?$map@...@std@@0@Z.
// Callees: as the console twin FinalConstructSubtitles 0x0059D0B8 ("PC_SUBTITLE" @ 0x01230428 instead of
// "UI_ONE_OFF_SUBTITLE"), with component virtual slot 60 (+0xF0) for the add and `list` virtual slot 124 (+0x1F0, the raw
// component pointer pushed from the counted pointer's object word) after it; both slot names are descriptive.
// Shape: the console twin's (see 0x0059D0B8): manager first, the stand-in `find` (same-TU noinline body as in the landed
// FinalRefreshSubtitles 0x0059D761), the name conversion homed in the dead `component` slot, the counted-pointer local.
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (engine/CCountedPointer.h).
// Naming (NF168, 2026-11-20): moved onto the shared headers. Own name: Ego_r.pdb / FableWin.pdb private
// ?FinalConstructSubtitlesPC@CTCInventoryTrade@@AAEXPAVCComponent@NUISystem@@AAV?$map@...@std@@0@Z (engine/CTCInventoryTrade.h opt-in
// FABLE_RETAIL_CTCINVENTORYTRADE_FINALSUBTITLES), the spelling the landed caller ConstructSubtitlesPC 0x0059E4D7 (it binds a stand-in today; see NOTES) binds; the map is the real STLport
// map<CComponent*, pair<CWideString,long> > whose _M_find 0x005B1A8D is called out of line (FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE, the
// NF50 trade-refresh form); component slots 9 / 142 / 144 / 59 / 60 / 124 are FindFirstChildOfType / CText::GetValue / CText::SetValue /
// AddChild / AddChildDynamically / AddPositionChild (engine/NUISystem_CComponent.h); callee 0x005CC772 is CTCInventoryStats::GetXString.
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

void CTCInventoryTrade::FinalConstructSubtitlesPC(CComponent* component, std::map<CComponent*,std::pair<CWideString,long> >& subtitles, CComponent* list)
{
    CManager* manager = CManager::GetInstance();
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
    CCountedPointer<CComponent> subtitle(manager->CreateComponent("PC_SUBTITLE", false));
    static_cast<CText*>(subtitle.Get())->SetValue(text, true, true);
    component->AddChildDynamically(subtitle);
    list->AddPositionChild(subtitle.Get());
}
