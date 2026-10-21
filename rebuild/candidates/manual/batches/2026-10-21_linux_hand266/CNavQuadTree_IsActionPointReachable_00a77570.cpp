// CNavQuadTree::IsActionPointReachable(const CNavigationPoint& point) -- an action point is reachable when its leaf node is unblocked, or when any unblocked leaf lies on the ring just outside the node's square (both sides of each column, then both sides of each row, in half-unit steps)
// Leaf: IsActionPointReachable
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A77570, 0x19C bytes (padded to 416), `ret 4`, `and esp,-8` frame.
// Callees: CNavQuadTree::GetNodeAt(const C2DVector*, long layer) 0x00A770B0. Data: node sizes per level 0x0129DCAC,
//   0.5f 0x0122F59C, 0.25f 0x0123078C (ring margin, a literal).
// Layout: local model: the point is a C2DVector plus the layer at +8; the leaf node keeps its centre at +4/+8, its level
//   byte at +0xc and the blocked flag at +0xf.
// Shape: the 8-byte probe vector whose address is passed inside the loops gives the aligned frame (GOTCHAS chunk 252).
struct C2DVector { float X, Y; };
struct CNavigationPoint {
    C2DVector Position;                             // +0x00
    long      Layer;                                // +0x08
};
struct CNavLeaf {
    void*         _vptr;
    C2DVector     Centre;                           // +0x04
    unsigned char Level;                            // +0x0c
    unsigned char _pad0d[2];
    bool          Blocked;                          // +0x0f
};
extern const float g_navNodeSizes[];                // 0x0129DCAC
class CNavQuadTree {
public:
    CNavLeaf* GetNodeAt(const C2DVector* position, long layer);   // 0x00A770B0
    bool IsActionPointReachable(const CNavigationPoint& point);
};
bool CNavQuadTree::IsActionPointReachable(const CNavigationPoint& point)
{
    CNavLeaf* node = GetNodeAt(&point.Position, point.Layer);
    if (!node)
        return false;
    if (!node->Blocked)
        return true;
    C2DVector centre = node->Centre;
    float halfSize = g_navNodeSizes[node->Level] * 0.5f;
    float reach = halfSize + 0.25f;
    float x = centre.X - reach;
    float endX = centre.X + reach;
    for (; x <= endX; x += 0.5f) {
        for (long side = -1; side <= 1; side += 2) {
            C2DVector probe;
            probe.X = x;
            probe.Y = side * reach + centre.Y;
            CNavLeaf* leaf = GetNodeAt(&probe, point.Layer);
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
            CNavLeaf* leaf = GetNodeAt(&probe, point.Layer);
            if (leaf && !leaf->Blocked)
                return true;
        }
    }
    return false;
}
