// NInventory::CTCInventoryBase::GetSelectedCategoryIndex() const -- the index of the selected category (SelectedCategoryIndex +0x70)
// Leaf: ?GetSelectedCategoryIndex@CTCInventoryBase@NInventory@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC4B2, 4 bytes: `mov eax,[ecx+70h] ; ret`.
// Name fix (agentNF89, q11167): Ego_r.pdb + FableWin.pdb ?GetSelectedCategoryIndex@CTCInventoryBase@NInventory@@QBEJXZ; engine/NInventory_CTCInventoryBase.h
//   (FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDCATEGORYINDEX) declares it at this address and the landed 0x00622A00 / 0x006223C0 bind it. Was the
//   placeholder _global::GetMember_dword0x70_005bc4b2 (leaf ?DrawGetEnvironment@@YIHPBE@Z, a __fastcall global reading +0x70: same bytes).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDCATEGORYINDEX
#include "engine/NInventory_CTCInventoryBase.h"
namespace NInventory {
long CTCInventoryBase::GetSelectedCategoryIndex() const
{
    return SelectedCategoryIndex;
}
}
