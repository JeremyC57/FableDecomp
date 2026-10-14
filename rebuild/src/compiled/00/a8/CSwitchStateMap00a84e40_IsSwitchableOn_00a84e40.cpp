// CSwitchStateMap_00a84e40::IsSwitchableOn(unsigned __int64 key) -- true when the 64-bit key is in the sorted vector map at +0x20 (LowerBound @ 0x00A84E80, then the end / `key < found` check with the high word compared first)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A84E40, length 64 bytes.
// Leaf: IsSwitchableOn
// The chunk label (CNavSwitchableLeafNode::IsSwitchableOn) is propagated; named by address. Shape: the map in esi, the two-word compare, the result as `setne cl ; mov al,cl`, `ret 8`.
struct SKeyEntry_00a84e40 {
    unsigned __int64 m_key;                        // +0x00
};
struct CVectorMap_00a84e40 {
    SKeyEntry_00a84e40* m_first;
    SKeyEntry_00a84e40* m_last;
    SKeyEntry_00a84e40* LowerBound(const unsigned __int64& key);   // 0x00A84E80
    SKeyEntry_00a84e40* Find(const unsigned __int64& key)         // the inline map find: end() read through the map, re-read by the caller
    {
        SKeyEntry_00a84e40* found = LowerBound(key);
        return (found == m_last || key < found->m_key) ? m_last : found;
    }
};

struct CSwitchStateMap_00a84e40 {
    unsigned char       _pad00[0x20];
    CVectorMap_00a84e40 m_states;                  // +0x20
    bool IsSwitchableOn(unsigned __int64 key);
};

bool CSwitchStateMap_00a84e40::IsSwitchableOn(unsigned __int64 key)
{
    return m_states.Find(key) != m_states.m_last;
}