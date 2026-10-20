// CNavQuadTree::IsAreaAllSamePreferability(const CANavQuadTreeNode* node) const -- whether every topology-weight cell under the node's square has the same preferability (always true for nodes on other layers or without a weight map)
// Leaf: IsAreaAllSamePreferability
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A76CC0, 305 bytes (+ int3 padding), `ret 4`.
// Callees: __ftol2 0x00BFEA70 (the float -> long cell conversions).
// Globals: the per-level node size table 0x0129DCAC (float[]), the 0.5f literal.
// Layout: engine/CNavQuadTree.h (Ego_r.pdb, packed): MapPos +7 (unaligned C2DVector), MapWidth +0x10; TopologyWeights at
//   retail +0x34 (12-byte STLport vectors before it). Modelled locally (the header carries opaque byte arrays).
// Shape: the node square computed first, then the cell walk nested under the layer / non-empty test (one shared
//   `return true` exit keeps every callee-saved push at entry); the cell loops compare `(float)index`
//   against loop-invariant far edges; the first cell's weight is kept in a byte local (homed in the dead node slot)
//   with a found flag in bl.
struct C2DVector { float X, Y; };
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
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
struct CByteVector {                               // STLport vector<unsigned char>
    unsigned char* _M_start;
    unsigned char* _M_finish;
    unsigned char* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    const unsigned char& operator[](long n) const { return _M_start[n]; }
};
#pragma pack(push, 1)
class CNavQuadTree {
public:
    virtual void v00();
    bool          Initialised;                     // +0x04
    bool          Activated;                       // +0x05
    bool          Initialising;                    // +0x06
    C2DVector     MapPos;                          // +0x07
    unsigned char _pad0f;
    float         MapWidth;                        // +0x10
    float         MapHeight;                       // +0x14
    long          MapIndex;                        // +0x18
    unsigned char NavigationLayers[0x0c];          // +0x1c
    unsigned char TransitionPoints[0x0c];          // +0x28
    CByteVector   TopologyWeights;                 // +0x34
    bool IsAreaAllSamePreferability(const CANavQuadTreeNode* node) const;
};
#pragma pack(pop)

bool CNavQuadTree::IsAreaAllSamePreferability(const CANavQuadTreeNode* node) const
{
    float half = g_QuadTreeLevelSizes[node->QuadTreeLevel] * 0.5f;
    C2DBoxF box;
    box.TLX = node->Position.X - half;
    box.TLY = node->Position.Y - half;
    box.BRX = half + node->Position.X;
    box.BRY = half + node->Position.Y;
    if (node->NavLayer == 0 && !TopologyWeights.empty()) {
        unsigned char preferability = 0xff;
        bool found = false;
        for (long x = (long)(box.TLX - MapPos.X); x < box.BRX - MapPos.X; ++x) {
            for (long y = (long)(box.TLY - MapPos.Y); y < box.BRY - MapPos.Y; ++y) {
                long index = (long)(y * MapWidth + x);
                if (!found) {
                    preferability = TopologyWeights[index];
                    found = true;
                } else if (TopologyWeights[index] != preferability)
                    return false;
            }
        }
    }
    return true;
}
