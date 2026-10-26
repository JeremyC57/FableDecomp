#include <cstdio>
// CNavQuadTreeNode::CNavQuadTreeNode(C2DVector const*,unsigned char,unsigned char,CAFile*) -- load four child pointers and initialise a navigation node
// Retail: Fable.exe @ 0x00A76580, 177 bytes. The base file reads are virtual slot 3.
// Shape: the polymorphic CANavQuadTreeNode base is constructed first, then the derived vtable and four zero children
// are installed. Each CAFile::Read call fills one child pointer through its local output slot.
// Clean CANavQuadTreeNode/CNavQuadTreeNode headers agree on every field offset, but flatten
// the hierarchy and provide no constructors or virtual destructor. This local method model
// restores the CBase and CANavQuadTreeNode construction required by the retail vtable writes.
// Leaf: ??0CNavQuadTreeNode
class C2DVector { public: float X, Y; C2DVector() {} C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {} };
class CAFile { public:
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual void Read(void* dst, unsigned long bytes, bool raw); // 0x00A7F880
};
struct CBase {
    virtual ~CBase();
    CBase(); // 0x0099A2F0
};
class CANavQuadTreeNode : public CBase { public:
    C2DVector Position;
    unsigned char QuadTreeLevel;
    unsigned char NavLayer;
    bool LeafNode;
    bool Blocked;
    CANavQuadTreeNode(const C2DVector& p, unsigned char level, unsigned char layer)
        : Position(p), QuadTreeLevel(level), NavLayer(layer), Blocked(false) {}
    virtual ~CANavQuadTreeNode();
};
class CNavQuadTreeNode : public CANavQuadTreeNode { public:
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

CBase::CBase() {}
CBase::~CBase() {}
CANavQuadTreeNode::~CANavQuadTreeNode() {}
void CAFile::v000() {}
void CAFile::v001() {}
void CAFile::v002() {}
void CAFile::Read(void*, unsigned long, bool) {}
struct TestFile : CAFile {
    CANavQuadTreeNode* values[4]; int calls; bool good;
    TestFile() : calls(0), good(true) {}
    virtual void Read(void* dst, unsigned long bytes, bool raw) {
        if (bytes != 4 || raw || calls >= 4) good = false;
        if (calls < 4) *(CANavQuadTreeNode**)dst = values[calls];
        ++calls;
    }
};
int main() {
    C2DVector p; p.X=1.25f; p.Y=-3.5f;
    TestFile f; f.values[0]=(CANavQuadTreeNode*)1; f.values[1]=(CANavQuadTreeNode*)2; f.values[2]=(CANavQuadTreeNode*)3; f.values[3]=(CANavQuadTreeNode*)4;
    CNavQuadTreeNode n(&p, 7, 9, &f);
    if (n.Position.X != p.X || n.Position.Y != p.Y || n.QuadTreeLevel != 7 || n.NavLayer != 9 ||
        n.LeafNode || n.Blocked || n.TLNode != f.values[0] || n.TRNode != f.values[1] || n.BLNode != f.values[2] || n.BRNode != f.values[3] ||
        f.calls != 4 || !f.good) { std::printf("00a76580_TEST FAIL\n"); return 1; }
    std::printf("00a76580_TEST PASS\n"); return 0;
}