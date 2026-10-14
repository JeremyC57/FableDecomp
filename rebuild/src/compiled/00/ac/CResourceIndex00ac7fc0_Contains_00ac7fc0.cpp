// CResourceIndex_00ac7fc0::Contains(unsigned long key) const -- the ordered tree at +0xa8 holds a key >= the argument (lower_bound != end)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC7FC0, length 41 bytes.
// Callees: the tree's FindGreaterEqual @ 0x00AD25D0 (`this` = the tree at +0xa8, the iterator returned through a hidden pointer homed in the
// 4-byte local, the key, a by-value argument whose slot address is passed: `lea eax,[esp+0Ch]`). Shape: `push ecx` opens the local, the result's node is compared with the tree's sentinel
// (its first word, read AFTER the call) and `setne` through `xor eax,eax` is the bool. The manifest label
// "CDisplayManager::CopyBackBufferToTexture" is propagated from a same-shape body; the class is named by address.
struct CRetailTreeIterator {
    void* m_pNode;
    CRetailTreeIterator(void* node) : m_pNode(node) { }
    CRetailTreeIterator(const CRetailTreeIterator& other) : m_pNode(other.m_pNode) { }
};

struct CRetailTree_00ac7fc0 {                      // VC6 Dinkumware tree: the head (sentinel) pointer first
    void* m_pHead;
    unsigned long m_size;
    CRetailTreeIterator FindGreaterEqual(const unsigned long& key) const;   // 0x00AD25D0 (lower_bound)
    void* End() const { return m_pHead; }
};

struct CResourceIndex_00ac7fc0 {
    unsigned char _pad[0xa8];
    CRetailTree_00ac7fc0 m_tree;                   // +0xa8
    bool Contains(unsigned long key) const;        // by value: its slot's address is what the tree call gets
};

bool CResourceIndex_00ac7fc0::Contains(unsigned long key) const
{
    CRetailTreeIterator found = m_tree.FindGreaterEqual(key);
    return found.m_pNode != m_tree.End();
}