// Grade: DIFFER(642v642) -- same length; only the three fadds of the height-test sum (ahead + offset) take their operands in the opposite order (retail fld ahead.X/Y then fadd the global, fld global.Z then fadd ahead.Z; ours the reverse in all three). Operand order in the source, a member vs free vs by-value operator+, an explicit component sum, static/defined/block-scope extern globals: VC7.1 picks the same order every time
// CNavigatorAStar::GetRandomNavigablePos(const C3DVector& pos, float distance, const C2DVector& direction, float spread) const -- a navigable point about `distance` along `direction` from pos: straight ahead when that node is reachable from pos's node (height from the interface), else the farthest of the navigable points along the direction and the direction turned by +/- half the spread
// Leaf: ?GetRandomNavigablePos@CNavigatorAStar@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1BA10, 656 bytes (`ret 14h`, C3DVector result by hidden pointer).
// Callees: the interface's slot 16 GetLayerAt (+0x40) and slot 4 GetGroundHeight (+0x10), CNavigatorManager::GetNodeAt
//   0x00A14F70, own slot 7 GetNearestNavigablePosTo (+0x1c), C2DVector::Normalise 0x00A14540, CANavQuadTreeNode::operator==
//   0x00A763F0 and slot 4 IsConnectedTo (+0x10), GetNavigablePosInDirection 0x00A1AD40, C2DVector::Rotate 0x00A141D0;
//   the height-test offset 0x013D27D0, 0.5 (double) 0x012316F0.
// Layout (retail): CNavigatorAStar PInterface +0x14, NavigationManager +0x1c; CANavQuadTreeNode Blocked +0x0f.
struct C2DVector;
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(o.X + X, o.Y + Y, o.Z + Z); }
};
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) {}
    C2DVector& Normalise();                        // 0x00A14540
    void Rotate(float angle);                      // 0x00A141D0
    float GetLengthSquared() const { return X * X + Y * Y; }
    C2DVector operator*(float f) const { return C2DVector(X * f, Y * f); }
};
struct CANavQuadTreeNode {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual bool IsConnectedTo(const CANavQuadTreeNode* other) const;   // slot 4 (+0x10)
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
    bool operator==(const CANavQuadTreeNode& other) const;              // 0x00A763F0
};
struct CNavigatorManager {
    CANavQuadTreeNode* GetNodeAt(const C2DVector& position, long layer);   // 0x00A14F70
};
struct CNavigatorExternalInterface {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual float GetGroundHeight(const C3DVector& position, bool exact);   // slot 4 (+0x10)
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetLayerAt(const C3DVector& position);                      // slot 16 (+0x40)
};
extern C3DVector g_NavHeightTestOffset;            // 0x013D27D0
struct CNavigatorAStar {
    // +0x00 vptr
    unsigned char                _pad04[0x10];
    CNavigatorExternalInterface* PInterface;       // +0x14
    unsigned char                _pad18[0x4];
    CNavigatorManager*           NavigationManager;// +0x1c
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual C3DVector GetNearestNavigablePosTo(const C3DVector& pos, float distance, bool b) const;   // slot 7 (+0x1c)
    virtual C3DVector GetRandomNavigablePos(const C3DVector& pos, float distance, const C2DVector& direction, float spread) const;
    C3DVector GetNavigablePosInDirection(const C3DVector& pos, const C2DVector& direction, float distance) const;   // 0x00A1AD40
};

C3DVector CNavigatorAStar::GetRandomNavigablePos(const C3DVector& pos, float distance, const C2DVector& direction, float spread) const
{
    CANavQuadTreeNode* node = NavigationManager->GetNodeAt(C2DVector(pos), PInterface->GetLayerAt(pos));
    if (node == 0)
        return pos;
    if (node->Blocked)
        return GetNearestNavigablePosTo(pos, distance, true);
    C2DVector dir = direction;
    dir.Normalise();
    {
    C2DVector offset = dir * distance;
    C3DVector ahead(pos.X + offset.X, pos.Y + offset.Y, pos.Z);
    CANavQuadTreeNode* aheadNode = NavigationManager->GetNodeAt(C2DVector(ahead), 0);
    if (aheadNode != 0 && (aheadNode == node || *aheadNode == *node || aheadNode->IsConnectedTo(node)))
        return C3DVector(ahead.X, ahead.Y, PInterface->GetGroundHeight(ahead + g_NavHeightTestOffset, true));
    }
    {
    C3DVector straight = GetNavigablePosInDirection(pos, dir, distance);
    dir.Rotate((float)(spread * 0.5));
    C3DVector left = GetNavigablePosInDirection(pos, dir, distance);
    dir.Rotate(-spread);
    C3DVector right = GetNavigablePosInDirection(pos, dir, distance);
    float straightDist = C2DVector(straight.X - pos.X, straight.Y - pos.Y).GetLengthSquared();
    float leftDist = C2DVector(left.X - pos.X, left.Y - pos.Y).GetLengthSquared();
    float rightDist = C2DVector(right.X - pos.X, right.Y - pos.Y).GetLengthSquared();
    if (straightDist >= leftDist && straightDist >= rightDist)
        return C3DVector(straight.X, straight.Y, straight.Z);
    if (leftDist > straightDist && leftDist > rightDist)
        return C3DVector(left.X, left.Y, left.Z);
    return C3DVector(right.X, right.Y, right.Z);
    }
}
