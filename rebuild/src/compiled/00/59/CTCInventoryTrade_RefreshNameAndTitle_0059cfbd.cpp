// CTCInventoryTrade::RefreshNameAndTitle(NUISystem::CComponent* entry, long defIndex, long count) -- write the item's name into the entry's MENU_ENTRY_TEXT and UI_TEXT_TRADE_NAME_TEMPLATE_NEW children, followed by " x N" (localised multiplier and count) for stacks of more than one non-special item
// Leaf: ?RefreshNameAndTitle@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@JJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059CFBD, length 251 bytes, `ret 0Ch`. Frame-pointer size TU: optimize("y",off) +
// ("s",on). FableWin.pdb ?RefreshNameAndTitle@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@JJ@Z: called from BuyItem
// (0x005AF64E) and the console sell path (0x005AECC0); the PC twin RefreshNameAndTitlePC is 0x0059CA95, the same body with
// "PC_SUBTITLE_INDEPENDANT".
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("MENU_ENTRY_TEXT" @ 0x01230B80, "UI_TEXT_TRADE_NAME_TEMPLATE_NEW" @
// 0x012302B8), NUISystem::CComponent virtual slots 4 (+0x10) and 8 (+0x20) (child lookups by name, CCharString by value) and
// 144 (+0x240, SetText(const CWideString&, bool, bool)), CWideString() @ 0x0099AED0 / operator= @ 0x0099B7D0 / operator+=
// @ 0x0099B8D0 / ~ @ 0x0099B510, CTCInventoryItem::GetDescription(long) @ 0x005D95C0 (static `__fastcall`, CWideString by hidden pointer; Ego_r.pdb
// name), CTCInventoryTrade::IsSpecialCase(long) @ 0x0059C983, GFIntToWideString(long) @ 0x00405F00, CTCInventoryStats::GetXString() @ 0x005CC772,
// operator+(const CWideString&, const CWideString&) @ 0x0099BE70 (`__fastcall`, hidden result).
// Shape: the landed FinalRefreshSubtitles (0x0059D761) model: the lookup names are implicit conversions built in place
// (`push ecx ; mov ecx,esp`, `this` parked in [ebp-8]); the name local lives in the dead `entry` slot and the count arm's
// temporaries in the dead `defIndex` / `count` slots and the parked-this word; ebx = 1 serves `count > 1` and both bools.
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
#define FABLE_RETAIL_CTCINVENTORYTRADE_BUYITEMPC
#define FABLE_RETAIL_CTCINVENTORYTRADE_BUYPOSSIBILITYPC
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
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

void CTCInventoryTrade::RefreshNameAndTitle(CComponent* entry, long defIndex, long count)
{
    CComponent* text = entry->FindChildForward("MENU_ENTRY_TEXT");
    CComponent* subtitle = entry->FindChildNonRecursive("UI_TEXT_TRADE_NAME_TEMPLATE_NEW");
    CWideString name;
    if (text || subtitle) {
        name = CTCInventoryItem::GetDescription(defIndex);
        if (count > 1 && !IsSpecialCase(defIndex))
            name += CTCInventoryStats::GetXString() + GFIntToWideString(count);
        if (text)
            static_cast<CText*>(text)->SetValue(name, true, true);
        if (subtitle)
            static_cast<CText*>(subtitle)->SetValue(name, true, true);
    }
}
