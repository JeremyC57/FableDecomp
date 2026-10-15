// CVertexGroupBlock::RemoveLast(unsigned long count) -- pop the last `count` entries of the pointer vector, erasing each one's key range from the owning tree before the pop
// Grade: DIFFER(ours v retail) -- 84v84: key `sub eax,4` for retail `add eax,-4` (`m_pLast - 1` and `&m_pLast[-1]` both give sub), the hidden-result pointer in ecx for retail's eax, and the two by-value iterator slots both reserved before being filled (retail fills the `second` slot right after its `push ecx`, having loaded `first` into edx early); tried: key by const& (copied into the frame, 92 bytes), key by value, `&m_pLast[-1]`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD9470, length 84 bytes.
// Callees: the tree's equal_range(const key&) @ 0x005D8676 (hidden-pointer pair<iterator, iterator> result,
// lower bound @ 0x005D8446 and upper bound @ 0x005D84F2 inside; manifest "CompareDataOffset::operator()" is
// propagated), erase(iterator first, iterator last) @ 0x005D8561 (manifest "CTCCarryable_RemoveAndDestroy",
// propagated) with each 4-byte iterator passed BY VALUE through a user copy constructor (built in the slot:
// `push ecx ; mov eax,esp ; mov [eax],value`).
// Shape: `count` in ebx with the unsigned entry test (`test ebx,ebx ; jbe`), the tree's address in edi, the
// 8-byte frame holding the pair, the key = `m_pLast - 1` (a POINTER value, the element address, pushed by
// value; a const-reference key would be copied into the frame first), the pair words re-read from the frame for the two
// by-value iterators (second then first), then `--m_pLast` as load/add -4/store, `dec ebx ; jne`.
// Layout: the vector's `last` at +4 (4-byte elements), the tree (VC6-era _Tree: head pointer) at +0xc.
struct SNode;

struct CNodeIterator {                         // 4-byte tree iterator with a user copy constructor
    SNode* m_pNode;
    CNodeIterator() { }
    CNodeIterator(const CNodeIterator& other) : m_pNode(other.m_pNode) { }
};

struct SIteratorPair {                         // pair<iterator, iterator>: non-POD, returned by hidden pointer
    CNodeIterator first;
    CNodeIterator second;
    SIteratorPair() { }
};

struct CVertexGroupTree {                      // the key -> group multimap at +0xc
    SNode* m_pHead;
    SIteratorPair equal_range(void* key);                             // 0x005D8676 (key BY VALUE: `push eax` of the pointer)
    void erase(CNodeIterator first, CNodeIterator last);              // 0x005D8561
};

struct CVertexGroupBlock {
    void*            m_pFirst;                 // +0x00
    void**           m_pLast;                  // +0x04 (one past the last 4-byte entry)
    void*            m_pEnd;                   // +0x08
    CVertexGroupTree m_groups;                 // +0x0c
    void RemoveLast(unsigned long count);
};

void CVertexGroupBlock::RemoveLast(unsigned long count)
{
    while (count > 0) {
        SIteratorPair range = m_groups.equal_range(&m_pLast[-1]);   // `&p[-1]` is `add eax,-4`; `p - 1` is `sub eax,4`
        m_groups.erase(range.first, range.second);
        --m_pLast;
        --count;
    }
}
