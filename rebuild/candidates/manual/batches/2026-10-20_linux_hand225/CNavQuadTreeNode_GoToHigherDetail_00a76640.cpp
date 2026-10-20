// CNavQuadTreeNode::GoToHigherDetail(const vector<C2DBoxF>& areas) const -- whether any corner of this node's square (centre Position, side from the per-level size table) lies inside one of the high-detail areas
// Leaf: GoToHigherDetail
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A76640, 289 bytes (+ int3 padding), `ret 4`; 8-aligned frame.
// Callees: C2DBoxF::ContainsPoint(const C2DVector&) 0x00535CD0 (out of line for the last two
//   corners; the first two expansions are inlined).
// Globals: the per-level node size table 0x0129DCAC (float[]), the 0.5f literal.
// Layout: CANavQuadTreeNode Position +4, QuadTreeLevel +0xc; C2DBoxF {TLX, TLY, BRX, BRY}; STLport vector (first, last).
// Shape: an early empty-range return, then the node box before the loop; the four corner tests are one
//   `||` chain: the first two through an inline float-pair Contains, the last two through the out-of-line ContainsPoint
//   with C2DVector temporaries (8-byte locals, hence the aligned frame).
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    bool Contains(float x, float y) const { return x >= TLX && x < BRX && y >= TLY && y < BRY; }
    bool ContainsPoint(const C2DVector& p) const;  // 0x00535CD0
};
struct C2DBoxFVector {                             // STLport vector<C2DBoxF>
    C2DBoxF* _M_start;
    C2DBoxF* _M_finish;
    C2DBoxF* _M_end_of_storage;
};
extern const float g_QuadTreeLevelSizes[];         // 0x0129DCAC

class CANavQuadTreeNode {
public:
    virtual void v00();
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
};
class CNavQuadTreeNode : public CANavQuadTreeNode {
public:
    const CANavQuadTreeNode* TLNode;               // +0x10
    const CANavQuadTreeNode* TRNode;               // +0x14
    const CANavQuadTreeNode* BLNode;               // +0x18
    const CANavQuadTreeNode* BRNode;               // +0x1c
    bool GoToHigherDetail(const C2DBoxFVector& areas) const;
};

bool CNavQuadTreeNode::GoToHigherDetail(const C2DBoxFVector& areas) const
{
    const C2DBoxF* area = areas._M_start;
    const C2DBoxF* end = areas._M_finish;
    if (area == end)
        return false;
    float half = g_QuadTreeLevelSizes[QuadTreeLevel] * 0.5f;
    C2DBoxF box;
    box.TLX = Position.X - half;
    box.TLY = Position.Y - half;
    box.BRX = half + Position.X;
    box.BRY = half + Position.Y;
    for (; area != end; ++area) {
        if (area->Contains(box.TLX, box.TLY) || area->Contains(box.BRX, box.TLY)
            || area->ContainsPoint(C2DVector(box.TLX, box.BRY)) || area->ContainsPoint(C2DVector(box.BRX, box.BRY)))
            return true;
    }
    return false;
}
