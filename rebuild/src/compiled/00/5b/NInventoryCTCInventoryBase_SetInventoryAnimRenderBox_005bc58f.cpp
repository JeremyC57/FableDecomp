// NInventory::CTCInventoryBase::SetInventoryAnimRenderBox(const C2DBoxF& box) -- store the screen box the inventory's 3D item animation is rendered into
// Leaf: ?SetInventoryAnimRenderBox@CTCInventoryBase@NInventory@@QAEXABVC2DBoxF@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BC58F, 35 bytes, `ret 4`; Ego_r.pdb ?SetInventoryAnimRenderBox@CTCInventoryBase@NInventory@@QAEXABVC2DBoxF@@@Z.
// Layout: InventoryAnimRenderBox (C2DBoxF) at +0xBC (Ego_r +0xD0; header overlay FABLE_RETAIL_CTCINVENTORYBASE_ANIMBOX).
// Shape: size TU (odd entry address: `optimize("s", on)`). The destination box is formed first (`lea eax,[ecx+0BCh]`) and the
//   four floats copied as dwords through edx: a reference to the member and four field stores. A plain `= box` is `rep movsd`
//   under /Os (21 bytes) and loads the source first under /O2; four direct member stores address [ecx+0BCh..] (42 bytes).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ANIMBOX
#define FABLE_RETAIL_STLP_VECTOR
#include "fable_string.h"
#include "engine/CTCBase.h"
#include "engine/NInventory_CTCInventoryBase.h"

void NInventory::CTCInventoryBase::SetInventoryAnimRenderBox(const C2DBoxF& box)
{
    C2DBoxF& dest = InventoryAnimRenderBox;
    dest.TLX = box.TLX;
    dest.TLY = box.TLY;
    dest.BRX = box.BRX;
    dest.BRY = box.BRY;
}