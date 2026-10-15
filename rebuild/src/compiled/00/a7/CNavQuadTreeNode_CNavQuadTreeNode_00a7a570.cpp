// CNavQuadTreeNode::CNavQuadTreeNode(const C2DVector&, unsigned char, unsigned char, const CNavQuadTree&, ENavQuadTreeNodeType&, vector<unsigned __int64>&) -- build an inner quad-tree node with no children yet and let Initialise subdivide it
// Leaf: ??0CNavQuadTreeNode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7A570, 98 bytes (+ int3 padding; `ret 18h`).
// Callees: CBase::CBase() @ 0x0099A2F0 (out of line, vtable 0x0129A7C4), CNavQuadTreeNode::Initialise(const CNavQuadTree&,
//   ENavQuadTreeNodeType&, vector<unsigned __int64>&) @ 0x00A79B30. Vtables: CANavQuadTreeNode 0x0129CBBC, CNavQuadTreeNode 0x0129DCD4.
// Layout = Ego_r.pdb: CANavQuadTreeNode {vptr, Position +4, QuadTreeLevel +0xc, NavLayer +0xd, LeafNode +0xe, Blocked +0xf},
//   CNavQuadTreeNode + TLNode..BRNode +0x10..+0x1c.
// Shape: the inline CANavQuadTreeNode ctor (its vtable store, the position through C2DVector's memberwise copy ctor, level,
//   layer, LeafNode = true, Blocked = false) merges with the derived initialisers (four null children); every store lands before
//   the derived vtable store. A LeafNode body assignment in the derived ctor stores after that vtable; the POD position copy
//   schedules the level store before the last argument load.
struct C2DVector {
    float X, Y;
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};
struct CBase {
    virtual ~CBase();
    CBase();                                       // 0x0099A2F0
};
struct CNavQuadTree;
enum ENavQuadTreeNodeType { NAV_NODE_QUADTREE = 0 };
namespace std {
template <class T> struct allocator { };
template <class T, class A = allocator<T> > struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };
}
struct CANavQuadTreeNode : CBase {
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
    CANavQuadTreeNode(const C2DVector& pos, unsigned char level, unsigned char layer)
        : Position(pos), QuadTreeLevel(level), NavLayer(layer), LeafNode(true), Blocked(false) { }
    virtual ~CANavQuadTreeNode();
};
struct CNavQuadTreeNode : CANavQuadTreeNode {
    CANavQuadTreeNode* TLNode;                     // +0x10
    CANavQuadTreeNode* TRNode;                     // +0x14
    CANavQuadTreeNode* BLNode;                     // +0x18
    CANavQuadTreeNode* BRNode;                     // +0x1c
    CNavQuadTreeNode(const C2DVector& pos, unsigned char level, unsigned char layer, const CNavQuadTree& tree,
                     ENavQuadTreeNodeType& type, std::vector<unsigned __int64>& switchableIds);
    virtual ~CNavQuadTreeNode();
protected:
    void Initialise(const CNavQuadTree& tree, ENavQuadTreeNodeType& type, std::vector<unsigned __int64>& switchableIds);   // 0x00A79B30
};

CNavQuadTreeNode::CNavQuadTreeNode(const C2DVector& pos, unsigned char level, unsigned char layer, const CNavQuadTree& tree,
                                   ENavQuadTreeNodeType& type, std::vector<unsigned __int64>& switchableIds)
    : CANavQuadTreeNode(pos, level, layer), TLNode(0), TRNode(0), BLNode(0), BRNode(0)
{
    Initialise(tree, type, switchableIds);
}