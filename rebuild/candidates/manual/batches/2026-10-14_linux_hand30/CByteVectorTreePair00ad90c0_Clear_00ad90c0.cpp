// CByteVectorTreePair00ad90c0::Clear() -- erase the whole byte vector (an inlined SGI erase(begin, end) whose copy degenerates to a zero-length memmove) and clear the STLport tree next to it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD90C0, length 83 bytes (+ int3 padding).
// Callees: memmove @ 0x00BFEAE6 (`__cdecl`, three arguments popped with `add esp,0Ch`; its result is used: the copy returns `memmove(...) + n`), the tree's
// recursive node erase @ 0x004507A1 (on the tree at +0xc with the root = header->parent; manifest "LTextTreeWalkThrough::BuildTreeArray" is propagated).
// The chunk label "CDrawQuestInfo::OracleSymbolPopBack" is propagated (the drawer has no such body); this 0x00ADxxxx object is a byte vector
// (first/last/end at +0..+8) followed by an STLport red-black tree (header pointer +0xc, node count +0x10; node: colour, parent, left, right),
// named by address. Shape: `last` and `m_pLast` are the same load, so the copy's early-out compare is `cmp eax,eax`; the memmove length is
// `sub esi,eax` of the same register; the tree is a named pointer (`lea esi,[edi+0Ch]`), its count test shares the edi zero register with the
// three reset stores, and the stores follow STLport's clear() order (leftmost, root, rightmost, count).
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

static inline unsigned char* CopyBytes(const unsigned char* first, const unsigned char* last, unsigned char* result)
{
    if (first == last)
        return result;
    unsigned int n = last - first;
    return (unsigned char*)memmove(result, first, n) + n;
}

struct CRetailByteVector {                         // SGI vector<unsigned char>
    unsigned char* m_pStart;
    unsigned char* m_pFinish;
    unsigned char* m_pEndOfStorage;
    void Erase(unsigned char* first, unsigned char* last)
    {
        m_pFinish = CopyBytes(last, m_pFinish, first);
    }
};

struct CRetailTree {                               // STLport _Rb_tree: allocated header node + count
    struct node { long m_colour; node* m_pParent; node* m_pLeft; node* m_pRight; };
    node*         m_pHeader;
    unsigned long m_nodeCount;
    void EraseSubtree(node* root);                 // 0x004507A1
    void Clear()
    {
        if (m_nodeCount != 0) {
            EraseSubtree(m_pHeader->m_pParent);
            m_pHeader->m_pLeft = m_pHeader;
            m_pHeader->m_pParent = 0;
            m_pHeader->m_pRight = m_pHeader;
            m_nodeCount = 0;
        }
    }
};

struct CByteVectorTreePair00ad90c0 {
    CRetailByteVector m_bytes;                     // +0x00
    CRetailTree       m_tree;                      // +0x0c
    void Clear();
};

void CByteVectorTreePair00ad90c0::Clear()
{
    m_bytes.Erase(m_bytes.m_pStart, m_bytes.m_pFinish);
    CRetailTree* tree = &m_tree;
    tree->Clear();
}
