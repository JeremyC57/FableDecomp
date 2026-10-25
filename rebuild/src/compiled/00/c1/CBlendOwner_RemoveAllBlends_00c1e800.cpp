// CBlendOwner::RemoveAllBlends() -- empties both blend lists by removing each list's last entry through RemoveBlend until its count is zero
// Leaf: RemoveAllBlends
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1E800, 72 bytes (`ret`). Descriptive name (no PDB covers this library): the
//   manifest's "C3DAnimationBlendState_RebuildCombinations" is propagated. Caller: the owner's destructor 0x00C1E850
//   (landed as CEngineScreenEffectOutlineGlow_00c1e850, which then releases the same two arrays' data blocks).
// Callees: RemoveBlend @ 0x00C1E7D0 (thiscall, one entry; landed under the wrong name "AddCombinationBlend": it removes
//   the entry from the +8 or +0x14 list through the array Remove 0x00C1E900).
// Shape: two rotated `while (count)` loops; the last entry is `data[count - 1]` (`mov edx,[ecx+eax*4-4]`), the count
//   re-read after every call.
struct CBlendEntry;

struct CBlendArray {                        // {data, capacity, count}
    CBlendEntry** m_data;
    unsigned long m_capacity;
    unsigned long m_count;
};

struct CBlendOwner {
    void* m_pOwner;                         // +0x00
    long m_4;                               // +0x04
    CBlendArray m_unkeyed;                  // +0x08
    CBlendArray m_keyed;                    // +0x14
    void RemoveBlend(CBlendEntry* entry);   // 0x00C1E7D0
    void RemoveAllBlends();
};

void CBlendOwner::RemoveAllBlends()
{
    while (m_unkeyed.m_count)
        RemoveBlend(m_unkeyed.m_data[m_unkeyed.m_count - 1]);
    while (m_keyed.m_count)
        RemoveBlend(m_keyed.m_data[m_keyed.m_count - 1]);
}