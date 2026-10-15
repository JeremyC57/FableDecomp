// SBinaryTreeNode_00ac39c0::`scalar deleting destructor'(unsigned int) -- delete both child nodes recursively, free the node's item array, then the flag-tested operator delete
// Leaf: ??_GSBinaryTreeNode_00ac39c0@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC39C0, length 69 bytes (+ int3 padding).
// Callees: itself (the children's `delete`, `push 1 ; call`), free @ 0x00BFEA14 (the inline array destructor), operator
// delete @ 0x00BFE9BC. The catalog label "CPatchTesselationEdgeStrip::SetMovableResourceHandler" is propagated; the only
// other caller (0x00AC2BE0, a gapscan body) deletes the root held at +0x3c of its owner with the same inlined sequence.
// The node class has no vtable and no RTTI, so it is named by address.
// Layout: a 0x44-byte node: payload +0x00..+0x30 (not touched here), the item array (a retail vector: first / last / end)
// +0x30, the two children +0x3c / +0x40.
// Shape: a NON-virtual `??_G` (`QAEPAXI@Z`), emitted because the translation unit `delete`s the class; the inline
// destructor body `delete m_pChildA; delete m_pChildB;` becomes `mov ecx,[esi+3Ch] ; test ; je ; push 1 ; call ??_G`,
// then the member array is freed, then `test byte ptr [esp+8],1 ; je ; push esi ; call operator delete`; `ret 4`.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct CItemArray_00ac39c0 {                       // retail vector of PODs: first / last / end
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    ~CItemArray_00ac39c0() { if (m_pFirst) free(m_pFirst); }
};

struct SBinaryTreeNode_00ac39c0 {
    unsigned char             _payload[0x30];      // +0x00
    CItemArray_00ac39c0       m_items;             // +0x30
    SBinaryTreeNode_00ac39c0* m_pChildA;           // +0x3c
    SBinaryTreeNode_00ac39c0* m_pChildB;           // +0x40
    ~SBinaryTreeNode_00ac39c0()
    {
        delete m_pChildA;
        delete m_pChildB;
    }
};

// A `delete` of the class makes the translation unit emit its scalar deleting destructor.
void DeleteTreeNode_00ac39c0(SBinaryTreeNode_00ac39c0* node)
{
    delete node;
}
