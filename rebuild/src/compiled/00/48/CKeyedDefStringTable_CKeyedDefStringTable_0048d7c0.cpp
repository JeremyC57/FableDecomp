// CKeyedDefStringTable::~CKeyedDefStringTable() -- implicit destructor of a non-polymorphic record holding an STLport map (header +0x10, count +0x14) and an STLport vector of (key, CDefString) pairs (+0x00): clear and free the tree, then destroy and free the vector
// Leaf: ??1CKeyedDefStringTable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048D7C0, length 103 bytes. Catalogued as
// CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures (a propagated label): its callers are the record's
// scalar deleting destructor @ 0x0048D870 and a `delete p` helper @ 0x0048D890; its constructor is 0x0048D830 (zeroes
// +0x00..+0x0c, mallocs the 0x20-byte tree header). The record's real name is not recoverable from the bodies.
// Callees: the tree's recursive _M_erase(root) @ 0x0047C6A0 (__fastcall on the tree), the pair range destroy @ 0x0047C800
// (__fastcall: first in ecx, last in edx, the empty allocator tag by reference on the stack; the manifest label
// "_Move_backward<pair<unsigned long,CDefString>*>" is propagated), free @ 0x00BFEA14.
// Shape: members die in reverse order (tree, then vector); STLport `clear()` = `if (count) { _M_erase(root); leftmost =
// header; root = 0; rightmost = header; count = 0; }` with the header re-read for every store; the header and the
// vector storage are freed only when non-null; the tag temporary sits in the high byte of the `push ecx` frame word.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct SScalarTag {
    SScalarTag() { }
};

struct SRbNodeBase {
    long         m_color;                          // +0x00
    SRbNodeBase* m_pParent;                        // +0x04 (the header's parent is the root)
    SRbNodeBase* m_pLeft;                          // +0x08 (the header's left is the leftmost)
    SRbNodeBase* m_pRight;                         // +0x0c (the header's right is the rightmost)
};

struct CRbTree_0048d7c0 {
    SRbNodeBase*  m_pHeader;                       // +0x00
    unsigned long m_nodeCount;                     // +0x04
    void Erase(SRbNodeBase* root);                 // 0x0047C6A0
    void clear()
    {
        if (m_nodeCount != 0) {
            Erase(m_pHeader->m_pParent);
            m_pHeader->m_pLeft = m_pHeader;
            m_pHeader->m_pParent = 0;
            m_pHeader->m_pRight = m_pHeader;
            m_nodeCount = 0;
        }
    }
    ~CRbTree_0048d7c0()
    {
        clear();
        if (m_pHeader)
            free(m_pHeader);
    }
};

struct SKeyedDefString {                           // pair<unsigned long, CDefString>
    unsigned long m_key;
    void*         m_pString;
};
void __fastcall DestroyRange(SKeyedDefString* first, SKeyedDefString* last, const SScalarTag& tag);   // 0x0047C800

struct CKeyedDefStringVector {
    SKeyedDefString* m_pFirst;
    SKeyedDefString* m_pLast;
    SKeyedDefString* m_pEnd;
    ~CKeyedDefStringVector()
    {
        DestroyRange(m_pFirst, m_pLast, SScalarTag());
        if (m_pFirst)
            free(m_pFirst);
    }
};

struct CKeyedDefStringTable {
    CKeyedDefStringVector m_entries;               // +0x00
    long                  m_word0c;                // +0x0c
    CRbTree_0048d7c0      m_index;                 // +0x10
    ~CKeyedDefStringTable();
};

CKeyedDefStringTable::~CKeyedDefStringTable()
{
}