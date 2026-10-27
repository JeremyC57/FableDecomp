// CNavQuadTree::$E_BlockedNode() -- dynamic initializer of CNavQuadTree::BlockedNode: the inline CNavBlockedQuadTreeNode(0, 0) constructor (position (-1,-1), level and layer 0, a blocked leaf), then its destructor is registered with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012243D0, 76 bytes, in `.text$yc`, reached from the .CRT$XCU table, plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb `dynamic initializer for 'CNavQuadTree::BlockedNode'`; the object 0x013D2D2C
//   gets the CNavBlockedQuadTreeNode vtable 0x0129DD3C (rtti-vtables.tsv) and is the node the navigation code
//   (0x00A14CB0 .. 0x00A7CB68, 33 references) compares and links against; the registered thunk 0x0122AAD0 is the
//   compiler-generated ~CNavBlockedQuadTreeNode (CANavQuadTreeNode vtable 0x0129CBBC, then ~CBaseClassNonCopyable 0x0099A300).
// Label: the manifest's "sub_012243d0" is a gap-scan placeholder; the landed thunk 0x0122AAD0 carries a propagated
//   `_Cons_val<...CParticleMorphs::CEntry...>` label.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable() 0x0099A2F0, _atexit 0x004012BC.
// Layout: Ego_r.pdb CANavQuadTreeNode: Position (C2DVector) +4, QuadTreeLevel +0xc, NavLayer +0xd, LeafNode +0xe, Blocked +0xf.
// Shape: the PDB constructors CNavBlockedQuadTreeNode(unsigned char, unsigned char) over
//   CANavQuadTreeNode(const C2DVector&, unsigned char, unsigned char) (landed 0x00A768C0 / 0x00A76370), inline: the
//   base's Blocked = false is dead under the derived body's Blocked = true; the base vtable store is dead.
class C2DVector {
public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};

class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable();                          // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                 // 0x0099A300
};

class CANavQuadTreeNode : public CBaseClassNonCopyable {    // vtable 0x0129CBBC
public:
    C2DVector     Position;                           // +0x04
    unsigned char QuadTreeLevel;                      // +0x0c
    unsigned char NavLayer;                           // +0x0d
    bool          LeafNode;                           // +0x0e
    bool          Blocked;                            // +0x0f
    CANavQuadTreeNode(const C2DVector& position, unsigned char level, unsigned char layer)
        : Position(position), QuadTreeLevel(level), NavLayer(layer), LeafNode(true), Blocked(false) { }
    virtual ~CANavQuadTreeNode() { }
};

class CNavBlockedQuadTreeNode : public CANavQuadTreeNode {  // vtable 0x0129DD3C
public:
    CNavBlockedQuadTreeNode(unsigned char level, unsigned char layer)
        : CANavQuadTreeNode(C2DVector(-1.0f, -1.0f), level, layer) { Blocked = true; }
};

class CNavQuadTree {
public:
    static CNavBlockedQuadTreeNode BlockedNode;       // 0x013D2D2C
};

CNavBlockedQuadTreeNode CNavQuadTree::BlockedNode(0, 0);