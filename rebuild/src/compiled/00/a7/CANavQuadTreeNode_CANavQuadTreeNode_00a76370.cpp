// CANavQuadTreeNode::CANavQuadTreeNode(const C2DVector&, unsigned char, unsigned char) -- base, vtable, position, level, layer, leaf, not blocked
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A76370, length 55 bytes (+ int3 padding).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable() @ 0x0099A2F0.
// Vtable 0x0129CBBC CANavQuadTreeNode (rtti-vtables.tsv). Members from engine/CANavQuadTreeNode.h:
// Position (C2DVector X, Y) +0x4, QuadTreeLevel +0xc, NavLayer +0xd, LeafNode +0xe, Blocked +0xf.
// Shape: base call, vtable, the position copied memberwise (ecx, edx: a user copy constructor;
// a POD copy schedules the level byte store before Position.Y), the two byte
// arguments, then LeafNode = true and Blocked = false, all in init-list order. `ret 0Ch`.
struct C2DVector {
    float X, Y;
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}   // memberwise: one register per dword
};

struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable();
    CBaseClassNonCopyable();                    // 0x0099A2F0
};

struct CANavQuadTreeNode : CBaseClassNonCopyable {
    C2DVector Position;                         // +0x04
    unsigned char QuadTreeLevel;                // +0x0c
    unsigned char NavLayer;                     // +0x0d
    bool LeafNode;                              // +0x0e
    bool Blocked;                               // +0x0f
    virtual void v01();                         // own vtable 0x0129CBBC
    CANavQuadTreeNode(const C2DVector& position, unsigned char level, unsigned char layer);
};

CANavQuadTreeNode::CANavQuadTreeNode(const C2DVector& position, unsigned char level, unsigned char layer)
    : CBaseClassNonCopyable(), Position(position), QuadTreeLevel(level), NavLayer(layer), LeafNode(true), Blocked(false)
{
}