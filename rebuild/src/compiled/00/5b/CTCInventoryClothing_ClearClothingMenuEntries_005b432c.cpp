// CTCInventoryClothing::ClearClothingMenuEntries() -- empty the clothing menu's {suit part, menu component} entry list
// Leaf: ?ClearClothingMenuEntries@CTCInventoryClothing@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B432C (optimize("s") TU).
// Identity: descriptive member name; ClothingMenuEntries list (overlay FABLE_RETAIL_CTCINVENTORYCLOTHING_MENUENTRIES).
// Shape: `ClothingMenuEntries.clear();` under `#pragma optimize("s", on)`.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_MENUENTRIES
#include "engine/CTCInventoryClothing.h"

void CTCInventoryClothing::ClearClothingMenuEntries()
{
    ClothingMenuEntries.clear();
}