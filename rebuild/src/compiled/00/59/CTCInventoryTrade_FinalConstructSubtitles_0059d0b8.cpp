// CTCInventoryTrade::FinalConstructSubtitles(NUISystem::CComponent* component, CSubtitleMap& subtitles) -- build a UI_ONE_OFF_SUBTITLE text component for `component` (its own type-6 child text when it has no entry in `subtitles`, otherwise the entry's text plus " x N" for a positive count) and add it as a child
// Leaf: ?FinalConstructSubtitles@CTCInventoryTrade@@AAEXPAVCComponent@NUISystem@@AAV?$map@PAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@4@@std@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059D0B8, length 249 bytes, `ret 8`. Frame-pointer size TU: optimize("y",off) +
// ("s",on). FableWin.pdb ?FinalConstructSubtitles@CTCInventoryTrade@@AAEXPAVCComponent@NUISystem@@AAV?$map@...@std@@@Z.
// Callees: NUISystem::CManager::GetInstance() @ 0x0041E5F2 (first), map<CComponent*, pair<CWideString,long> >::find
// @ 0x005B1A8D, CWideString() @ 0x0099AED0 / operator= @ 0x0099B7D0 / operator+= @ 0x0099B8D0 / ~ @ 0x0099B510, GetXString()
// @ 0x005CC772, GFIntToWideString(long) @ 0x00405F00, CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0 ("UI_ONE_OFF_SUBTITLE" @
// 0x0124E72C), CManager::CreateComponent(const CCharString&, bool) @ 0x0041DB1D, CCountedPointer<CComponent>(CComponent*) @
// 0x00429C15 / ~ @ 0x004291DE, component virtual slots 9 (+0x24), 142 (+0x238), 144 (+0x240 SetText) and 59 (+0xEC AddChild).
// Shape: the landed FinalRefreshSubtitles (0x0059D761) text logic and stand-in `find` (same-TU noinline body, as accepted
// there), the landed CTCInventoryStats::CreateTitlePC (0x005CCFFC) creation: the manager fetched into a local before the
// lookup, the definition name an implicit conversion homed in the dead `component` slot, the new component held in a
// counted-pointer local through its inline operator->.
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (engine/CCountedPointer.h).
// Naming (NF168, 2026-11-20): moved onto the shared headers. Own name: Ego_r.pdb / FableWin.pdb private
// ?FinalConstructSubtitles@CTCInventoryTrade@@AAEXPAVCComponent@NUISystem@@AAV?$map@...@std@@@Z (engine/CTCInventoryTrade.h opt-in
// FABLE_RETAIL_CTCINVENTORYTRADE_FINALSUBTITLES), the spelling the landed caller ConstructSubtitles 0x0059DF49 binds; the map is the real STLport
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

void CTCInventoryTrade::FinalConstructSubtitles(CComponent* component, std::map<CComponent*,std::pair<CWideString,long> >& subtitles)
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
    CCountedPointer<CComponent> subtitle(manager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));
    static_cast<CText*>(subtitle.Get())->SetValue(text, true, true);
    component->AddChild(subtitle);
}
