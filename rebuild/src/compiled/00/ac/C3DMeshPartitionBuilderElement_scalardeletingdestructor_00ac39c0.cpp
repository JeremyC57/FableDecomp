// C3DMeshPartitionBuilderElement::`scalar deleting destructor'(unsigned int) -- delete both sub-elements recursively, free the triangle array, then the flag-tested operator delete
// Leaf: ??_GC3DMeshPartitionBuilderElement@@QAEPAXI@Z
// Identity (naming wave 34): Ego_r.pdb C3DMeshPartitionBuilderElement (0x48 bytes: StartTriangle, TriangleCount, BoundingBox,
// BoundingSphere, Triangles CArray<C3DTriangle2> +0x30, SubElements[2] +0x40; retail 0x44 with the 12-byte CArray: SubElements
// +0x3C / +0x40) with the non-virtual ??1C3DMeshPartitionBuilderElement@@QAE@XZ in Ego_r / FableWin; Codex pass 12 binds the
// ??_G from the builder 0x00AC2BE0. Was landed as SBinaryTreeNode_00ac39c0 (named by address).
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

template <class T> class CArray { public:            // retail CArray: first / last / end (12 bytes)
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    ~CArray() { if (m_pFirst) free(m_pFirst); }
};
class C3DTriangle2;

class C3DMeshPartitionBuilderElement { public:
    unsigned char                    _payload[0x30];     // +0x00 StartTriangle, TriangleCount, BoundingBox, BoundingSphere
    CArray<C3DTriangle2>             Triangles;          // +0x30
    C3DMeshPartitionBuilderElement*  SubElements[2];     // +0x3c / +0x40
    ~C3DMeshPartitionBuilderElement()
    {
        delete SubElements[0];
        delete SubElements[1];
    }
};

// A `delete` of the class makes the translation unit emit its scalar deleting destructor.
void DeleteTreeNode_00ac39c0(C3DMeshPartitionBuilderElement* node)
{
    delete node;
}