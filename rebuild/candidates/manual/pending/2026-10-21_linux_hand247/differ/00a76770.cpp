// Grade: DIFFER(318v321) -- VC7.1 keeps the QuadTreeLevel byte from the size-table index in dl for the four blocked branches (pos goes to esi, no tail calls); retail reloads it in each branch and tail-jumps
// CNavQuadTreeNode::GetLeafNodeAt(const C2DVector&) const -- descend into the quadrant that holds the point; a blocked quadrant answers with the shared blocked node repositioned to the quadrant's centre one level down
// Leaf: GetLeafNodeAt
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A76770, length 321 bytes (padded to 336); CNavQuadTreeNode vtable 0x0129DCD4 slot 11.
// Callees: virtual GetLeafNodeAt (slot 11) on the chosen child, as a tail call (`jmp [eax+2Ch]`).
// Globals: per-level cell size table float[] @ 0x0129DCAC (32, 16, 8, ...), 0.25f @ 0x0123078C, the shared
//          CNavBlockedQuadTreeNode @ 0x013D2D2C (Position +4/+8 -> 0x013D2D30/34, QuadTreeLevel 0x013D2D38, NavLayer 0x013D2D39).
// Layout: engine/CNavQuadTreeNode.h (TLNode +0x10, TRNode +0x14, BLNode +0x18, BRNode +0x1c) over the PDB CANavQuadTreeNode
// base (vptr, Position +4, QuadTreeLevel +0xc, NavLayer +0xd, LeafNode +0xe, Blocked +0xf); restated with the virtual slots.
// Shape: quarter = size[level] * 0.25f lives in the `push ecx` slot; x < X picks the left column, y < Y the top row. The
// blocked node's centre is set through an inline C2DVector::Set(x, y) (arguments built right to left: Y first, stored X then Y).
struct C2DVector {
    float X;
    float Y;
    void Set(float x, float y) { X = x; Y = y; }
};

extern float g_quadTreeLevelCellSize[];                            // 0x0129DCAC

class CANavQuadTreeNode {
public:
    virtual ~CANavQuadTreeNode();                                  // slot 0
    virtual void Slot1();
    virtual void Slot2();
    virtual void Slot3();
    virtual void Slot4();
    virtual void Slot5();
    virtual void Slot6();
    virtual void Slot7();
    virtual void Slot8();
    virtual void Slot9();
    virtual void Slot10();
    virtual const CANavQuadTreeNode* GetLeafNodeAt(const C2DVector& pos) const;   // slot 11

    C2DVector     Position;                                        // +0x04
    unsigned char QuadTreeLevel;                                   // +0x0c
    unsigned char NavLayer;                                        // +0x0d
    bool          LeafNode;                                        // +0x0e
    bool          Blocked;                                         // +0x0f
};

class CNavBlockedQuadTreeNode : public CANavQuadTreeNode {
};

extern CNavBlockedQuadTreeNode g_BlockedNavNode;                   // 0x013D2D2C

class CNavQuadTreeNode : public CANavQuadTreeNode {
public:
    virtual const CANavQuadTreeNode* GetLeafNodeAt(const C2DVector& pos) const;

    CANavQuadTreeNode* TLNode;                                     // +0x10
    CANavQuadTreeNode* TRNode;                                     // +0x14
    CANavQuadTreeNode* BLNode;                                     // +0x18
    CANavQuadTreeNode* BRNode;                                     // +0x1c
};

const CANavQuadTreeNode* CNavQuadTreeNode::GetLeafNodeAt(const C2DVector& pos) const
{
    float quarter = g_quadTreeLevelCellSize[QuadTreeLevel] * 0.25f;
    if (pos.X < Position.X) {
        if (pos.Y < Position.Y) {
            if (TLNode == &g_BlockedNavNode) {
                g_BlockedNavNode.QuadTreeLevel = QuadTreeLevel + 1;
                g_BlockedNavNode.NavLayer = NavLayer;
                g_BlockedNavNode.Position.Set(Position.X - quarter, Position.Y - quarter);
                return &g_BlockedNavNode;
            }
            return TLNode->GetLeafNodeAt(pos);
        }
        if (BLNode == &g_BlockedNavNode) {
            g_BlockedNavNode.QuadTreeLevel = QuadTreeLevel + 1;
            g_BlockedNavNode.NavLayer = NavLayer;
            g_BlockedNavNode.Position.Set(Position.X - quarter, Position.Y + quarter);
            return &g_BlockedNavNode;
        }
        return BLNode->GetLeafNodeAt(pos);
    }
    if (pos.Y < Position.Y) {
        if (TRNode == &g_BlockedNavNode) {
            g_BlockedNavNode.QuadTreeLevel = QuadTreeLevel + 1;
            g_BlockedNavNode.NavLayer = NavLayer;
            g_BlockedNavNode.Position.Set(Position.X + quarter, Position.Y - quarter);
            return &g_BlockedNavNode;
        }
        return TRNode->GetLeafNodeAt(pos);
    }
    if (BRNode == &g_BlockedNavNode) {
        g_BlockedNavNode.QuadTreeLevel = QuadTreeLevel + 1;
        g_BlockedNavNode.NavLayer = NavLayer;
        g_BlockedNavNode.Position.Set(Position.X + quarter, Position.Y + quarter);
        return &g_BlockedNavNode;
    }
    return BRNode->GetLeafNodeAt(pos);
}
