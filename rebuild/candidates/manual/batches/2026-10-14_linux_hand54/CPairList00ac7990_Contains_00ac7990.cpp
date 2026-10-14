// CPairList_00ac7990::Contains(void* item) -- true when any 8-byte pair of the vector at +4 holds the item in either slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC7990, length 72 bytes.
// Leaf: Contains
// The chunk label (CList::SetDefaultChild) is propagated. Shape: the index in edx compared unsigned against `size()` (re-read each iteration), the pair pointer stepped by 8 in eax, the item in esi, `ret 4`.
struct SPair_00ac7990 {                            // 8 bytes
    void* m_first;
    void* m_second;
};
struct CRetailVector_00ac7990 {
    SPair_00ac7990* m_first;
    SPair_00ac7990* m_last;
    SPair_00ac7990* m_end;
    unsigned long size() const { return m_last - m_first; }
};

struct CPairList_00ac7990 {
    long                   m_0;
    CRetailVector_00ac7990 m_pairs;                // +0x04
    bool Contains(void* item);
};

bool CPairList_00ac7990::Contains(void* item)
{
    for (unsigned long i = 0; i < m_pairs.size(); ++i) {
        if (m_pairs.m_first[i].m_first == item || m_pairs.m_first[i].m_second == item)
            return true;
    }
    return false;
}
