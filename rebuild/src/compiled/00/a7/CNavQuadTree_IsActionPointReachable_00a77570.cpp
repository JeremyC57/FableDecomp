// CNavQuadTree::IsActionPointReachable(const CNavigationPosition& point) const -- an action point is reachable when its leaf node is unblocked, or when any unblocked leaf lies on the ring just outside the node's square (both sides of each column, then both sides of each row, in half-unit steps)
// Leaf: ?IsActionPointReachable@CNavQuadTree@@ABE_NABVCNavigationPosition@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A77570, 0x19C bytes (padded to 416), `ret 4`, `and esp,-8` frame.
// Callees: CNavQuadTree::GetNodeAt(const C2DVector&, long layer) const 0x00A770B0. Data: g_QuadTreeLevelSizes 0x0129DCAC,
//   0.5f 0x0122F59C, 0.25f 0x0123078C (ring margin, a literal).
// Layout: local model: CNavigationPosition (Ego_r: Position C2DVector, Layer +8); the node is CANavQuadTreeNode (Ego_r: Position
//   +4/+8, QuadTreeLevel +0xc, Blocked +0xf).
// Names (landing 2026-11-03g, chunk 736 NAME_FIXES): Ego_r.pdb ?IsActionPointReachable@CNavQuadTree@@ABE_NABVCNavigationPosition@@@Z
//   (private const) and ?GetNodeAt@CNavQuadTree@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@J@Z; the size table is the descriptive
//   g_QuadTreeLevelSizes of the landed 0x00A76640 / 0x00A76CC0 rows (no PDB data symbol). Were CNavigationPoint / CNavLeaf /
//   g_navNodeSizes and the non-const pointer GetNodeAt; the bytes are unchanged.
// Shape: the 8-byte probe vector whose address is passed inside the loops gives the aligned frame (GOTCHAS chunk 252).
class C2DVector { public: float X, Y; };
class CNavigationPosition { public:
    C2DVector Position;                             // +0x00
    long      Layer;                                // +0x08
};
class CANavQuadTreeNode { public:
    void*         __vftable;                        // +0x00
    C2DVector     Position;                         // +0x04
    unsigned char QuadTreeLevel;                    // +0x0c
    unsigned char NavLayer;                         // +0x0d
    bool          LeafNode;                         // +0x0e
    bool          Blocked;                          // +0x0f
};
extern const float g_QuadTreeLevelSizes[];          // 0x0129DCAC
class CNavQuadTree {
public:
    const CANavQuadTreeNode* GetNodeAt(const C2DVector& position, long layer) const;   // 0x00A770B0
private:
    bool IsActionPointReachable(const CNavigationPosition& point) const;
};
bool CNavQuadTree::IsActionPointReachable(const CNavigationPosition& point) const
{
    const CANavQuadTreeNode* node = GetNodeAt(point.Position, point.Layer);
    if (!node)
        return false;
    if (!node->Blocked)
        return true;
    C2DVector centre = node->Position;
    float halfSize = g_QuadTreeLevelSizes[node->QuadTreeLevel] * 0.5f;
    float reach = halfSize + 0.25f;
    float x = centre.X - reach;
    float endX = centre.X + reach;
    for (; x <= endX; x += 0.5f) {
        for (long side = -1; side <= 1; side += 2) {
            C2DVector probe;
            probe.X = x;
            probe.Y = side * reach + centre.Y;
            const CANavQuadTreeNode* leaf = GetNodeAt(probe, point.Layer);
            if (leaf && !leaf->Blocked)
                return true;
        }
    }
    float y = centre.Y - halfSize + 0.25f;
    float endY = centre.Y + reach;
    for (; y < endY; y += 0.5f) {
        for (long side = -1; side <= 1; side += 2) {
            C2DVector probe;
            probe.Y = y;
            probe.X = side * reach + centre.X;
            const CANavQuadTreeNode* leaf = GetNodeAt(probe, point.Layer);
            if (leaf && !leaf->Blocked)
                return true;
        }
    }
    return false;
}
