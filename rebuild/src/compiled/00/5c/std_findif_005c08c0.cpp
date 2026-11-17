// std::find_if<const NInventory::CCategory*, NInventory::CIsInventoryCategory>(first, last, pred) -- STLport 4.6 _algo.h: forwards to the
//   random-access __find_if 0x005C0653 with the pointer's iterator-category tag (by const reference: `lea eax,[ebp+0Bh] ; push eax`)
// Leaf: ??$find_if@PBVCCategory@NInventory@@VCIsInventoryCategory@2@@std@@YIPBVCCategory@NInventory@@PBV12@0VCIsInventoryCategory@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C08C0, 19 bytes, `ret 4` (/Gr __fastcall: first in ecx, last in edx, the 4-byte predicate by value on
//   the stack); size-optimised TU.
// Identity (NF88, q11159): every caller is a const NInventory::CTCInventoryBase member that searches Categories (+0x20) with the def index of
//   the item's category (GetBoughtPrice 0x005BEAD5, GetPerItemDataForItem 0x005BE8D3, IsSpaceInInventoryForItem 0x005BF2ED,
//   GetAmountOfItemsContainable 0x005BF30D, GetNumberOfItemsOfTypeInInventory 0x005BDF08: all QBE in Ego_r.pdb), hence the const pointer
//   instantiation; the non-const twin is 0x005C08D3 -> 0x005C06C3. Ego_r.pdb / FableWin.pdb carry find_if / _Find_if instances over
//   CCategory with NInventory::CIsInventoryCategory (FableWin 0x02376C93 ??$_Find_if@PBVCCategory@NInventory@@VCIsInventoryCategory@2@@std@@).
//   Descriptive by library (STLport; both PDBs are Dinkumware builds). Was the stand-in std::vector<EWeaponClass>::_Umove<EWeaponClass*>
//   (leaf _FableUmove_005c08c0@4, an extern "C" __stdcall model).
// Port (NF88): one explicit instantiation of stlport/fable_algorithm.h find_if with the NF88 opt-in FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS
//   (the __find_if declaration) over engine/CCategory.h (PASS22 class + NF88 opt-in FABLE_RETAIL_CCATEGORY_ITEMQUERIES: the predicate).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
class CThing;                                   // engine/CCategory.h's PASS22 class names CIntelligentPointer<CThing>
#include "engine/CCategory.h"
#include "stlport/fable_algorithm.h"

template const NInventory::CCategory* FABLE_FASTCALL std::find_if(const NInventory::CCategory*, const NInventory::CCategory*,
    NInventory::CIsInventoryCategory);
