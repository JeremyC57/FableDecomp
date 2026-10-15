// CTCInventoryExperience::vfunc_29() -- refresh the four menu-arrow highlight states while spending experience
// Grade: DIFFER(66v66) -- register allocation only: retail reuses the compare-zero edx as the setg target and holds first/limit in esi/edi with the second result in eax; the lane takes ebx for both setcc results (ternary, (cond)+1, named first/limit locals all identical)
// Retail 0x0056EF2D, 66 bytes (vtable 0x0124F108 slot 29; called through `jmp [eax+74h]` by the
// slot-38/39 input handlers 0x0056EC8C/0x0056ECCB).
// When InventorySelectionState (+0x1C) is 1 the category arrows (+0x38/+0x3C) are set to 1 and the
// item arrows get 2 (lit) / 1: up lit when the first display entry is > 0, down lit when the last
// display entry (`v[size() - 1]`, size from `(finish - start) >> 2`) is below statIndicatorMax - 1.
// `cond ? 2 : 1` is `setg/setl + inc`; the constant 1 lives in eax (`xor eax,eax ; inc eax`, /Os).
// The vector is reached through a named pointer local (`lea eax,[ecx+208h]`, then `[eax]`, `[eax+4]`)
// and its start pointer is a second named local (retail keeps it in esi across the +0x40 store; a
// `list->_M_start` re-read after that store is not CSE'd).
// Base members (retail offsets, 12-byte STLport vectors): InventorySelectionState +0x1C,
// MenuArrowHighlightsCategoryUp/Dn/ItemUp/ItemDn +0x38..+0x44 (PDB +0x40..+0x4C);
// statIndicatorMax +0x1C0, MenuDisplayListExperienceItem +0x208 (PDB +0x204/+0x258). Local model.
#pragma optimize("s", on)

struct SLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
};

struct CTCInventoryExperience {
    void* __vftable;                            // +0x000
    unsigned char _pad_004[0x18];
    long InventorySelectionState;               // +0x01C
    unsigned char _pad_020[0x18];
    long MenuArrowHighlightsCategoryUp;         // +0x038
    long MenuArrowHighlightsCategoryDn;         // +0x03C
    long MenuArrowHighlightsItemUp;             // +0x040
    long MenuArrowHighlightsItemDn;             // +0x044
    unsigned char _pad_048[0x178];
    long statIndicatorMax;                      // +0x1C0
    unsigned char _pad_1c4[0x44];
    SLongVector MenuDisplayListExperienceItem;  // +0x208
    void vfunc_29();
};

void CTCInventoryExperience::vfunc_29()
{
    if (InventorySelectionState == 1) {
        MenuArrowHighlightsCategoryUp = 1;
        MenuArrowHighlightsCategoryDn = 1;
        SLongVector* list = &MenuDisplayListExperienceItem;
        long* first = list->_M_start;
        long lastAllowed = statIndicatorMax - 1;
        MenuArrowHighlightsItemUp = first[0] > 0 ? 2 : 1;
        MenuArrowHighlightsItemDn = first[(list->_M_finish - first) - 1] < lastAllowed ? 2 : 1;
    }
}
