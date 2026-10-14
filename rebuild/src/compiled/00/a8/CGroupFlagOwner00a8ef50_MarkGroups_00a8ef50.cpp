// CGroupFlagOwner_00a8ef50::MarkGroups(const long* index) -- for every 8-byte pair of the vector at +0x1c (its first word an owner), when the owner's own vector (+4..+8, 8-byte elements) is not empty set the byte at `index` of the owner's flag array (+0) to 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8EF50, length 80 bytes.
// Leaf: MarkGroups
// The chunk label (CMeshDataBank::FreeDeviceSpecificResources) is propagated; named by address. Shape: `this` stays in ecx, the index pointer in esi re-read each time, the `size() != 0` test as `test edi,0FFFFFFF8h`, `ret 4`.
struct SItem_00a8ef50 { long m_0, m_4; };
struct CGroup_00a8ef50 {
    unsigned char*  m_pFlags;                      // +0x00
    SItem_00a8ef50* m_first;                       // +0x04
    SItem_00a8ef50* m_last;                        // +0x08
    unsigned long size() const { return m_last - m_first; }
};
struct SGroupEntry_00a8ef50 {                      // 8 bytes
    CGroup_00a8ef50* m_pGroup;
    long             m_4;
};
struct CRetailVector_00a8ef50 {
    SGroupEntry_00a8ef50* m_first;
    SGroupEntry_00a8ef50* m_last;
    SGroupEntry_00a8ef50* m_end;
    unsigned long size() const { return m_last - m_first; }
};

struct CGroupFlagOwner_00a8ef50 {
    unsigned char          _pad00[0x1c];
    CRetailVector_00a8ef50 m_groups;               // +0x1c
    void MarkGroups(const long* index);
};

void CGroupFlagOwner_00a8ef50::MarkGroups(const long* index)
{
    for (unsigned long i = 0; i < m_groups.size(); ++i) {
        CGroup_00a8ef50* group = m_groups.m_first[i].m_pGroup;
        if (group->size() != 0)
            group->m_pFlags[*index] = 1;
    }
}