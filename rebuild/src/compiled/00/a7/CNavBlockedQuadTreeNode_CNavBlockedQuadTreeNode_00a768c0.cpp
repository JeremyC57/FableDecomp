// CNavBlockedQuadTreeNode::CNavBlockedQuadTreeNode(bool c, bool d) -- the CANavQuadTreeNode base inline (CBaseClassNonCopyable @ 0x0099A2F0; x and y set to -1.0f, the bytes +0xc / +0xd from the arguments, +0xe / +0xf set), then this class's vtable (0x0129DD3C)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A768C0, length 64 bytes.
// Vtable 0x0129DD3C names the class. Shape: `this` in esi, -1.0f through one integer register for both stores, the vtable store sunk below the member stores, `ret 8`.
struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                       // 0x0099A2F0
};
struct CANavQuadTreeNode : CBaseClassNonCopyable { // vtable 0x0129CBBC
    virtual void v00();
    float m_x;                                     // +0x04
    float m_y;                                     // +0x08
    bool  QuadTreeLevel;                                     // +0x0c
    bool  NavLayer;                                     // +0x0d
    bool  LeafNode;                                     // +0x0e
    bool  Blocked;                                     // +0x0f
    CANavQuadTreeNode(float x, float y, bool c, bool d) : CBaseClassNonCopyable(), m_x(x), m_y(y), QuadTreeLevel(c), NavLayer(d), LeafNode(true), Blocked(true) { }
};

struct CNavBlockedQuadTreeNode : CANavQuadTreeNode {   // vtable 0x0129DD3C
    virtual void v00();
    CNavBlockedQuadTreeNode(bool c, bool d);
};

CNavBlockedQuadTreeNode::CNavBlockedQuadTreeNode(bool c, bool d)
    : CANavQuadTreeNode(-1.0f, -1.0f, c, d)
{
}