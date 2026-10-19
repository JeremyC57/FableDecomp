// CNavQuadTreeNode::CNavQuadTreeNode(C2DVector const*,unsigned char,unsigned char,CAFile*) -- load four child pointers and initialise a navigation node
// Retail: Fable.exe @ 0x00A76580, 0x70 bytes. The base file reads are virtual slot 3.
// Shape: the polymorphic CANavQuadTreeNode base is constructed first, then the derived vtable and four zero children
// are installed. Each CAFile::Read call fills one child pointer through its local output slot.
// Leaf: ??0CNavQuadTreeNode
struct C2DVector { float X, Y; };
struct CAFile {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual void Read(void* dst, unsigned long bytes, bool raw); // 0x00A7F880
};
struct CBase {
    virtual ~CBase();
    CBase(); // 0x0099A2F0
};
struct CANavQuadTreeNode : CBase {
    C2DVector Position;
    unsigned char QuadTreeLevel;
    unsigned char NavLayer;
    bool LeafNode;
    bool Blocked;
    CANavQuadTreeNode(const C2DVector& p, unsigned char level, unsigned char layer)
        : Position(p), QuadTreeLevel(level), NavLayer(layer), Blocked(false) {}
    virtual ~CANavQuadTreeNode();
};
struct CNavQuadTreeNode : CANavQuadTreeNode {
    CANavQuadTreeNode* TLNode;
    CANavQuadTreeNode* TRNode;
    CANavQuadTreeNode* BLNode;
    CANavQuadTreeNode* BRNode;
    CNavQuadTreeNode(const C2DVector* p, unsigned char level, unsigned char layer, CAFile* file);
};
CNavQuadTreeNode::CNavQuadTreeNode(const C2DVector* p, unsigned char level, unsigned char layer, CAFile* file)
    : CANavQuadTreeNode(*p, level, layer), TLNode(0), TRNode(0), BLNode(0), BRNode(0)
{
    CANavQuadTreeNode* child0;
    CANavQuadTreeNode* child1;
    CANavQuadTreeNode* child2;
    CANavQuadTreeNode* child3;
    LeafNode = false;
    file->Read(&child0, 4, false); TLNode = child0;
    file->Read(&child1, 4, false); TRNode = child1;
    file->Read(&child2, 4, false); BLNode = child2;
    file->Read(&child3, 4, false); BRNode = child3;
}
