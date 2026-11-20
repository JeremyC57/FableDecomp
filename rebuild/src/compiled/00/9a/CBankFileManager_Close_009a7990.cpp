// CBankFileManager::Close() -- drop every retail bank (popping the counted pointers off the back of the list) and clear the bank map
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A7990, 135 bytes (+9 int3), __fastcall, no arguments.
// Callees: the count block's destroy function (`call [blk+4]`, object in ecx: __fastcall), operator delete
// 0x00BFE9BC (count block), free 0x00BFEA14 (list node), the Banks tree's recursive _Erase 0x009AA7C0.
// Layout: Ego_r.pdb CBankFileManager with retail containers -- Banks, a VC6-era map {head, size} plus its
// comparator word, at +4 (head +4, size +8); RetailBanks, the one-word sentinel list of
// CCountedPointer<NBankFileManager::CRetailBank> (node: next, prev, object +8, count block +0x0C), at +0x10.
// Shape: `while (!empty()) pop_back();` with the list head re-read every pass: the last node is unlinked
// (next/prev read first), its counted pointer destroyed inline (release re-reading the block, both words
// cleared) and the node freed; then the inline map clear(): a non-empty tree is erased from the root and
// the head's left/parent/right and the size reset (the head re-read for each store).
// Leaf: ?Close@CBankFileManager@@QAEXXZ
// NF132 (q6186): moved onto the retail fable_retail_banks.h CBankFileManager (Banks = std::map<CCharString, CBankInfo>, RetailBanks =
//   std::list<CCountedPointer<NBankFileManager::CRetailBank> >) and the shared STLport list / map (stlport/fable_containers.h /
//   fable_map.h); the tree's out-of-line erase 0x009AA7C0 is now ?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@...
//   (was the local CBankTree::_Erase stand-in over local CRetailList / SBankNode / CBankTree / CBankFileManager models).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CBANKFILEMANAGER
#define FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_POPBACK
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "fable_retail_banks.h"

void CBankFileManager::Close()
{
    while (!RetailBanks.empty())
        RetailBanks.pop_back();
    Banks.clear();
}
