// NInventory::CTCInventoryBase::AddItemToInventory(long itemDefIndex, long count, bool add_selected, bool add_quick_access, long
//   price_bought_for, bool silent) -- add items of a type: a hero title sets the owner's title instead (false); otherwise the category of the
//   item's inventory category takes them and OnItemAdded is raised (true)
// Leaf: ?AddItemToInventory@CTCInventoryBase@NInventory@@UAE_NJJ_N0J0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF32D, length 153 bytes. Frame-pointer size TU: optimize("s", on).
// Callees: CTCInventoryItem::IsHeroTitle 0x005D98F0 (static __fastcall; was bound as the made-up ?IsAugmentation_005d98f0@@YI_NJ@Z),
//   CTCInventoryItem::GetInventoryCategory 0x005D97B0, std::bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (the inline
//   CThing::GetTC<CTCHero>), CTCHero::SetTitle 0x006CA400, std::find_if<CCategory*, CIsInventoryCategory> 0x005C08D3 (__fastcall; the
//   non-const twin of 0x005C08C0), NInventory::CCategory::AddItem(long, bool, long, long) 0x005BF060, virtual slot 84 OnItemAdded (+0x150).
// Shape: the inline GetTC keeps the key in the dead slot of the item index (held in ebx); the not-found and title paths share the `xor al,al`
//   exit; no end check after find_if.
// Identity (NF92, q11211): Ego_r.pdb 0x006220E6 + FableWin.pdb 0x02365FDA ?AddItemToInventory@CTCInventoryBase@NInventory@@UAE_NJJ_N0J0@Z
//   (public virtual, the 6-argument overload); parameter names after Ego_d.pdb's (CThing&) overload. Was the local
//   CTCInventoryBase_Retail model (leaf ?AddItemToInventory@CTCInventoryBase_Retail@@QAE_NJJ_N0J0@Z) with local CTCHero / CCategory /
//   CTCInventoryItem / find stand-ins; now on engine/NInventory_CTCInventoryBase.h (VTABLE, ALL_ITEMS_CHAIN_PASS22), CCategory.h, CThing.h,
//   CTCHero.h and CTCInventoryItem.h (NF92 opt-in ISHEROTITLE).
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_CCATEGORY_ADDITEM
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CTCINVENTORYITEM_ISHEROTITLE
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCHero.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CTCInventoryItem.h"
#include "stlport/fable_algorithm.h"

namespace NInventory {
bool CTCInventoryBase::AddItemToInventory(long itemDefIndex, long count, bool add_selected, bool add_quick_access, long price_bought_for, bool silent)
{
    if (CTCInventoryItem::IsHeroTitle(itemDefIndex)) {
        CTCHero* ptc_hero;
        if (GetParentThing().GetTC(&ptc_hero))
            ptc_hero->SetTitle(itemDefIndex);
    } else {
        CCategory* category = std::find_if(Categories.begin(), Categories.end(),
                                           CIsInventoryCategory(CTCInventoryItem::GetInventoryCategory(itemDefIndex)));
        if (category->AddItem(itemDefIndex, add_selected, price_bought_for, count)) {
            OnItemAdded(itemDefIndex, add_quick_access, silent);
            return true;
        }
    }
    return false;
}
}
