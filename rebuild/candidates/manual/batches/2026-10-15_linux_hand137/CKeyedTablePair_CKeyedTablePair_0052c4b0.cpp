// CKeyedTablePair::~CKeyedTablePair() -- implicit destructor of a non-polymorphic record holding two STLport vectors of the same 8-byte element (+0x00, +0x0c) and an STLport map (header +0x18, count +0x1c): clear and free the tree, then destroy and free the second and the first vector
// Leaf: ??1CKeyedTablePair
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052C4B0, length 135 bytes. Catalogued as
// CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures (a propagated label): its callers (0x005234A2, 0x00524892) are
// the record's deleting destructors; the record's real name is not recoverable from the bodies.
// Callees: the tree's recursive _M_erase(root) @ 0x00527860 (__thiscall on the tree), the element range destroy @ 0x0052BD70
// (__fastcall: first in ecx, last in edx, the empty allocator tag by reference on the stack, twice), free @ 0x00BFEA14.
// Shape: the same STLport member destructors as the record destructor @ 0x0048D7C0 (members die in reverse order; STLport
// `clear()` re-reads the header for every store; each block is freed only when non-null; the tag temporaries share the
// high byte of the `push ecx` frame word).
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

struct CRbTree_0052c4b0 {
    SRbNodeBase*  m_pHeader;                       // +0x00
    unsigned long m_nodeCount;                     // +0x04
    void Erase(SRbNodeBase* root);                 // 0x00527860
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
    ~CRbTree_0052c4b0()
    {
        clear();
        if (m_pHeader)
            free(m_pHeader);
    }
};

struct SKeyedDefString {                           // 8-byte element with a non-trivial destructor (destroyed out of line)
    unsigned long m_key;
    void*         m_pString;
};
void __fastcall DestroyRange(SKeyedDefString* first, SKeyedDefString* last, const SScalarTag& tag);   // 0x0052BD70

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

struct CKeyedTablePair {
    CKeyedDefStringVector m_first;                 // +0x00
    CKeyedDefStringVector m_second;                // +0x0c
    CRbTree_0052c4b0      m_index;                 // +0x18 (header +0x18, count +0x1c)
    ~CKeyedTablePair();
};

CKeyedTablePair::~CKeyedTablePair()
{
}
