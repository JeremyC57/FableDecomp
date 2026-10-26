#pragma optimize("s",on)
// Layout: retail STLport 4.6 header offsets; member names re-bound by the bytes this body touches: `GlobalDefIndex` -> `SelectedItemIndex` (+0x20), `SelectedItemIndex` -> `HighlightedItemIndex` (+0x24).
#include "engine/CCategory.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCategory_Methods : CCategory {
    void SetSelectedItemIndex(long index);
};

void CCategory_Methods::SetSelectedItemIndex(long index)
{
    this->SelectedItemIndex = index;
    this->HighlightedItemIndex = index;
    if (index == -1)
        this->HighlightedItemIndex = 0;
}