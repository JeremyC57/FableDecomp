// CNamedShaderTable::~CNamedShaderTable() -- implicit destructor of a non-polymorphic record holding an STLport vector of CCharString names (+0x00) and an STLport map (header +0x0c, count +0x10): clear and free the tree, then destroy every name and free the vector storage
// Leaf: ??1CNamedShaderTable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098D9F0, length 113 bytes. Catalogued as
// CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures (a propagated label): its only caller is the destructor
// @ 0x0098ABF0 (catalogued "~CPatchTesselationEdgeStrip", next to the CShaderRenderManager constructor @ 0x0098AB00),
// on its member at +0x3b4; the record's real name is not recoverable from the bodies.
// Callees: the tree's recursive _M_erase(root) @ 0x0043020A (__thiscall on the tree), ~CCharString @ 0x0099EAE0 (per name),
// free @ 0x00BFEA14.
// Shape: members die in reverse order (tree, then vector); STLport `clear()` re-reads the header for every store; the
// vector's `_Destroy(first, last)` is inlined as a pointer loop (first in esi, last in edi, 4-byte stride) calling the
// out-of-line string destructor; the storage pointer is re-read for the null-tested free.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CCharString {
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};

struct SRbNodeBase {
    long         m_color;                          // +0x00
    SRbNodeBase* m_pParent;                        // +0x04 (the header's parent is the root)
    SRbNodeBase* m_pLeft;                          // +0x08 (the header's left is the leftmost)
    SRbNodeBase* m_pRight;                         // +0x0c (the header's right is the rightmost)
};

struct CRbTree_0098d9f0 {
    SRbNodeBase*  m_pHeader;                       // +0x00
    unsigned long m_nodeCount;                     // +0x04
    void Erase(SRbNodeBase* root);                 // 0x0043020A
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
    ~CRbTree_0098d9f0()
    {
        clear();
        if (m_pHeader)
            free(m_pHeader);
    }
};

struct CCharStringVector {
    CCharString* m_pFirst;
    CCharString* m_pLast;
    CCharString* m_pEnd;
    static void Destroy(CCharString* first, CCharString* last)
    {
        for (; first != last; ++first)
            first->~CCharString();
    }
    ~CCharStringVector()
    {
        Destroy(m_pFirst, m_pLast);
        if (m_pFirst)
            free(m_pFirst);
    }
};

struct CNamedShaderTable {
    CCharStringVector m_names;                     // +0x00
    CRbTree_0098d9f0  m_index;                     // +0x0c
    ~CNamedShaderTable();
};

CNamedShaderTable::~CNamedShaderTable()
{
}
