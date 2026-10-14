// CTreeOwner_00868ca0::SetEntry(long key, float value) -- when `value` is above -1e-4, insert the (key, value) pair into the tree at +0x30 (0x004619BA, the iterator result discarded)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00868CA0, length 64 bytes.
// The chunk label "CIME::SetEditBoxRegion" is propagated; named by address. Float 0x0129BA3C (1e-4f, negated with `fchs`). Shape: a 16-byte frame (the pair at +0, the iterator at +8), `add ecx,30h`, `ret 8`.
extern const float g_epsilon_0129ba3c;            // 0x0129BA3C (1e-4f)
struct SPair_00868ca0 { long m_key; float m_value; };
struct SIterator_00868ca0 { void* m_pNode; long m_1; };   // 8 bytes
struct CTree_00868ca0 {
    unsigned char _body[8];
    SIterator_00868ca0 Insert(const SPair_00868ca0& entry);   // 0x004619BA (labelled "RBTree_LowerBound")
};
struct CTreeOwner_00868ca0 {
    unsigned char   _pad[0x30];
    CTree_00868ca0  m_tree;                        // +0x30
    void SetEntry(long key, float value);
};

void CTreeOwner_00868ca0::SetEntry(long key, float value)
{
    if (-g_epsilon_0129ba3c < value) {
        SPair_00868ca0 entry;
        entry.m_key = key;
        entry.m_value = value;
        m_tree.Insert(entry);
    }
}
