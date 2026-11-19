// NInventory::CPerItemData::CPerItemData() -- default per-item data: empty augmentation vector, zero counters, flag set, ref count -1
// Leaf: ??0CPerItemData@NInventory@@QAE@XZ
// Retail: Fable.exe @ 0x005C14A6, 30 bytes. Catalog label "CNavQuadTree::CNavigationLayer::CNavigationLayer" is wrong (ICF
// propagation): every caller (CItem::AddItem 0x005BECC7, CItem::UpdateItemStats 0x005BEE29/0x005BEEF2, ...) does
// `operator new(0x24)` then this ctor and hands the result to the item's CCountedPointer<CPerItemData> (+0x08) ShareData.
// Identity (NF93, q11232): FableWin.pdb ??0CPerItemData@NInventory@@QAE@XZ (0x0235E399; Ego_r.pdb keeps only the copy ctor
// ??0CPerItemData@NInventory@@QAE@ABV01@@Z), the body (the Ego_r.pdb CPerItemData members it initialises) and the landed callers
// 0x005BEC92 / 0x005BEDCB, which bind this address under that spelling.
// Port (NF93): was a strong out-of-line definition on a local NInventory::CPerItemData / vector model; now the header's inline ctor
// (engine/NInventory_CTCInventoryBase.h FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA_CTOR, which the callers already use), emitted here as
// its pick-any COMDAT, so one definition serves every includer.
// Opt: size TU (`or dword ptr [eax+1Ch],0FFFFFFFFh`); the STLport _Vector_base/_STLP_alloc_proxy ctor chain keeps the vector zeros
// ahead of the -1 store.
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA_CTOR
#include "engine/NInventory_CTCInventoryBase.h"

namespace {
// Fidelity: scaffolding, not retail: the smallest out-of-line use of the header's inline constructor (a size-optimised `new`, the form
// the CItem callers use), so VC7.1 emits ??0CPerItemData@NInventory@@QAE@XZ as a pick-any COMDAT.
NInventory::CPerItemData* ReferencePerItemDataCtor()
{
    return new NInventory::CPerItemData;
}
}
