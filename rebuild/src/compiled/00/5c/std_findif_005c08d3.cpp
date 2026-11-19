// std::find_if<NInventory::CCategory*, NInventory::CIsInventoryCategory>(first, last, pred) -- STLport 4.6 _algo.h: forwards to the
//   random-access __find_if 0x005C06C3 with the pointer's iterator-category tag (by const reference: `lea eax,[ebp+0Bh] ; push eax`)
// Leaf: ??$find_if@PAVCCategory@NInventory@@VCIsInventoryCategory@2@@std@@YIPAVCCategory@NInventory@@PAV12@0VCIsInventoryCategory@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C08D3, 19 bytes, `ret 4` (/Gr __fastcall: first in ecx, last in edx, the 4-byte predicate by value on
//   the stack); size-optimised TU.
// Identity (NF93, q11239): the non-const twin of 0x005C08C0 (NF88: find_if<const CCategory*, CIsInventoryCategory>): the same bytes,
//   forwarding to its own __find_if 0x005C06C3. Every caller is a non-const NInventory::CTCInventoryBase member that searches Categories
//   (+0x20) on the non-const vector (Ego_r.pdb QAE: AddItemToInventory 0x005BF32D / 0x005BF654, RemoveItemFromInventory 0x005BF3C6,
//   UpdateItemStatsForThing 0x005BF414, AddInventoryItem 0x005BF4A3, SelectItemWithDefIndex 0x005BDFB1); the ported 0x005BF32D (NF92) binds
//   this address so. Ego_r.pdb / FableWin.pdb carry the Dinkumware find_if / _Find_if instances over CCategory with
//   NInventory::CIsInventoryCategory. Descriptive by library (STLport). Was the stand-in std::vector<EWeaponClass>::_Umove<EWeaponClass*>
//   (leaf _FableUmove_005c08d3@4, an extern "C" __stdcall model).
// Port (NF93): one explicit instantiation of stlport/fable_algorithm.h find_if, the 0x005C08C0 recipe with the non-const iterator.
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

template NInventory::CCategory* FABLE_FASTCALL std::find_if(NInventory::CCategory*, NInventory::CCategory*,
    NInventory::CIsInventoryCategory);
