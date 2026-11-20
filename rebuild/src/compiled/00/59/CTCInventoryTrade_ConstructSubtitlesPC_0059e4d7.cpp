// CTCInventoryTrade::ConstructSubtitlesPC(NUISystem::CComponent* menu, CSubtitleMap& subtitles, NUISystem::CComponent* list) -- PC layout: build the subtitle of each of the thirteen trading-menu category entries (melee .. others) found by name under `menu`
// Leaf: ?ConstructSubtitlesPC@CTCInventoryTrade@@AAEXPAVCComponent@NUISystem@@AAV?$map@PAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@4@@std@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059E4D7, 486 bytes, `ret 0Ch`. Frame-pointer size TU: optimize("y",off) +
//   ("s",on). Ego_r.pdb: ?ConstructSubtitlesPC@CTCInventoryTrade@@AAEXPAVCComponent@NUISystem@@AAV?$map@...@std@@0@Z.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, the menu's virtual FindChild(CCharString) (slot 3,
//   +0x0C, as the landed FinalConstructSubtitlesPC 0x0059D65A models it), FinalConstructSubtitlesPC @ 0x0059D65A.
// Shape: thirteen `FinalConstructSubtitlesPC(menu->FindChild("<entry>"), subtitles, list)` calls: the outer arguments are pushed
//   first, then the menu's vptr is loaded and the by-value CCharString argument is built in its stack slot (`push ecx ; mov
//   ecx,esp ; push -1 ; push offset`) from the implicit const char* conversion; the callee destroys it.
// Strings: PC_TRADING_MENU_{MELEE,RANGED,HEAD,TORSO,HANDS,LEGS,FEET,SUITS,TROPHIES,POTIONS,GIFTS,PRODUCE,OTHERS}_ENTRY
//   (0x01252E34 .. 0x01252D38, read from the exe).
// Naming (NF168, 2026-11-20): moved onto the shared headers (engine/CTCInventoryTrade.h FABLE_RETAIL_CTCINVENTORYTRADE_PCINVENTORY declares
// this private member, FABLE_RETAIL_CTCINVENTORYTRADE_FINALSUBTITLES the callee); own leaf from the stand-in QAE / CSubtitleMap& form to the
// Ego_r.pdb private AAE map& spelling, which the landed callers 0x005A4CDD / 0x005A5B31 bind; callee 0x0059D65A FinalConstructSubtitlesPC
// respelled the same way.
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
#define FABLE_RETAIL_CTCINVENTORYTRADE_PCINVENTORY
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

void CTCInventoryTrade::ConstructSubtitlesPC(CComponent* menu, std::map<CComponent*,std::pair<CWideString,long> >& subtitles, CComponent* list)
{
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_MELEE_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_RANGED_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_HEAD_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_TORSO_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_HANDS_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_LEGS_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_FEET_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_SUITS_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_TROPHIES_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_POTIONS_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_GIFTS_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_PRODUCE_ENTRY"), subtitles, list);
    FinalConstructSubtitlesPC(menu->FindChild("PC_TRADING_MENU_OTHERS_ENTRY"), subtitles, list);
}