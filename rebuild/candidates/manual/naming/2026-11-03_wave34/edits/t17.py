P="rebuild/src/compiled/00/ac/SBinaryTreeNode00ac39c0_scalardeletingdestructor_00ac39c0.cpp"
EDITS=[
(P,"// SBinaryTreeNode_00ac39c0::`scalar deleting destructor'(unsigned int) -- delete both child nodes recursively, free the node's item array, then the flag-tested operator delete\n// Leaf: ??_GSBinaryTreeNode_00ac39c0@@QAEPAXI@Z",
 "// C3DMeshPartitionBuilderElement::`scalar deleting destructor'(unsigned int) -- delete both sub-elements recursively, free the triangle array, then the flag-tested operator delete\n// Leaf: ??_GC3DMeshPartitionBuilderElement@@QAEPAXI@Z\n// Identity (naming wave 34): Ego_r.pdb C3DMeshPartitionBuilderElement (0x48 bytes: StartTriangle, TriangleCount, BoundingBox,\n// BoundingSphere, Triangles CArray<C3DTriangle2> +0x30, SubElements[2] +0x40; retail 0x44 with the 12-byte CArray: SubElements\n// +0x3C / +0x40) with the non-virtual ??1C3DMeshPartitionBuilderElement@@QAE@XZ in Ego_r / FableWin; Codex pass 12 binds the\n// ??_G from the builder 0x00AC2BE0. Was landed as SBinaryTreeNode_00ac39c0 (named by address)."),
(P,"""struct CItemArray_00ac39c0 {                       // retail vector of PODs: first / last / end
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
""","""template <class T> class CArray { public:            // retail CArray: first / last / end (12 bytes)
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
"""),
(P,"void DeleteTreeNode_00ac39c0(SBinaryTreeNode_00ac39c0* node)","void DeleteTreeNode_00ac39c0(C3DMeshPartitionBuilderElement* node)"),
]
