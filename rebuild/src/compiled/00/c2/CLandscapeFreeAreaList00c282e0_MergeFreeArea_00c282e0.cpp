// CLandscapeFreeAreaList_00c282e0::MergeFreeArea(SFreeArea* area) -- coalesce a freed area with its neighbours: when the next area (+0x1c) is free (state byte +0xe == 'N') it is merged into this one (0x00C27F80 with (area, next)), then when the previous area (+0x18) is free this area is merged into it and that merge's result returned; otherwise the area itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C282E0, length 64 bytes.
// Callees: 0x00C27F80 (`MergeBlocks(first, second)`, thiscall, returns the surviving area). The chunk label (CLandscapeLayerMesh::OnPostRelocation) is propagated. Shape: the area in esi, `this` in edi, `ret 4`.
struct SFreeArea_00c282e0 {
    unsigned char       _pad00[0xe];
    char                m_state;                   // +0x0e: 'N' = not in use (free)
    unsigned char       _pad0f[9];
    SFreeArea_00c282e0* m_pPrev;                   // +0x18
    SFreeArea_00c282e0* m_pNext;                   // +0x1c
};

struct CLandscapeFreeAreaList_00c282e0 {
    SFreeArea_00c282e0* MergeBlocks(SFreeArea_00c282e0* first, SFreeArea_00c282e0* second);   // 0x00C27F80
    SFreeArea_00c282e0* MergeFreeArea(SFreeArea_00c282e0* area);
};

SFreeArea_00c282e0* CLandscapeFreeAreaList_00c282e0::MergeFreeArea(SFreeArea_00c282e0* area)
{
    if (area->m_pNext && area->m_pNext->m_state == 'N')
        MergeBlocks(area, area->m_pNext);
    if (area->m_pPrev && area->m_pPrev->m_state == 'N')
        return MergeBlocks(area->m_pPrev, area);
    return area;
}