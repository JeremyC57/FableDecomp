// CEngineLandscapeRenderer::UnregisterSubPatchForZSorting(long* slot) -- clears the sorter entry `*slot` of the sorted sub-patch array (+0x5e4: entries of a pointer and a flag word), drops its bit 0 and marks the slot -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B677A0, length 42 bytes (48 with padding).
// Callees: none. Shape: the entry index and the array pointer are re-read for the second store (the first store may alias either), the flag clear is
// `and dword ptr [ecx],-2` on the word at entry+4, then the slot is set to -1. engine/CEngineLandscapeRenderer.h (PDB) puts SortedSubPatchList at +0xe64;
// the retail renderer keeps the array's data pointer at +0x5e4 (the PC layout is narrower), so the member is modelled locally.
struct CSubPatchSorterEntry {
    void*         m_pSubPatch;                     // +0x00
    unsigned long m_flags;                         // +0x04 bit 0: registered
};

struct CEngineLandscapeRenderer {
    unsigned char         _pad[0x5e4];
    CSubPatchSorterEntry* m_pSortedSubPatches;     // +0x5e4
    void UnregisterSubPatchForZSorting(long* slot);
};

void CEngineLandscapeRenderer::UnregisterSubPatchForZSorting(long* slot)
{
    m_pSortedSubPatches[*slot].m_pSubPatch = 0;
    m_pSortedSubPatches[*slot].m_flags &= ~1;
    *slot = -1;
}