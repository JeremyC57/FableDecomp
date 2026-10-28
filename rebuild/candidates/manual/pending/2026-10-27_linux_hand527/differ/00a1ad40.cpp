// CNavigatorAStar::GetPosInDirection(const C3DVector& pos, const C2DVector& direction, float distance) const -- walk from pos along direction in half-node steps while the quad-tree node underfoot is open and the walked 2D distance stays within `distance`; return the last open position with its Z re-read from the external interface
// Grade: DIFFER(370v370) -- same length; residue: retail pushes ebx in the prologue (ours shrink-wraps it after the first node test), retail's frame has `current` above `last` (+0x28 / +0x1c, ours swapped), the step adds as `fadd [current.X]` with step.X on the stack (ours `fld current.X ; fadd st(1)`), and the `> distance` exit is a cold `fstp st(0) ; jmp` after the `ret`. 3 grades (probe/1ad40_a..c).
// Leaf: ?GetPosInDirection@CNavigatorAStar@@ABE?AVC3DVector@@ABV2@ABVC2DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1AD40, 370 bytes, thiscall, `ret 10h` (hidden result, pos, direction, distance).
// Identity: Ego_r.pdb `?GetPosInDirection@CNavigatorAStar@@ABE?AVC3DVector@@ABV2@ABVC2DVector@@M@Z` (private const). Ego_r donor
//   order GetNearestNavigablePosTo (landed 0x00A1AB40), GetPosInDirection, GetMyNodeAt (landed 0x00A1AEC0),
//   IsNavLineOfSightClear (0x00A1AF00, two C3DVector refs, bool, `ret 8`) matches retail; the only callers are three sites
//   0x00A1BB70 / 0x00A1BB9A / 0x00A1BBC0 inside GetRandomNavigablePos(const C3DVector&, float, const C2DVector&, float)
//   0x00A1BA10 (straight, left, right). The manifest label "CNavigatorAStar_GetRandomNavigablePos" names the caller; the
//   pending 2026-10-22 hand292 differ files call it the invented "GetNavigablePosInDirection".
// Callees: the external interface's (+0x14) slot 16 GetLayerAt and slot 4 FindPosZAt(pos, true), CNavigatorManager::GetNodeAt
//   0x00A14F70 (inlined GetMyNodeAt, twice), CANavQuadTreeNode::GetSize 0x00A75B30, 0.5f 0x0122F59C, the height-test offset
//   C3DVector 0x013D27D0 (BSS, no PDB name: descriptive).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
};
extern C3DVector g_NavHeightTestOffset;            // 0x013D27D0
class CANavQuadTreeNode {
public:
    virtual void v00();
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
    float GetSize() const;                         // 0x00A75B30
};
class CNavigatorManager {
public:
    CANavQuadTreeNode* GetNodeAt(const C2DVector& position, bool layer);   // 0x00A14F70
};
class CNavigatorExternalInterface {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual float FindPosZAt(const C3DVector& pos, bool useNavigation) const;   // slot 4 (+0x10)
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual bool GetLayerAt(const C3DVector& position) const;                    // slot 16 (+0x40)
};
class CNavigatorBase {
public:
    virtual void v00();
    unsigned char                _pad04[0x10];
    CNavigatorExternalInterface* PInterface;       // +0x14
    bool                         OnRoute;          // +0x18
    bool                         Interrupted;      // +0x19
    CNavigatorManager*           NavigationManager;   // +0x1c
    void*                        SteeringAgent;    // +0x20
};
class CNavigatorAStar : public CNavigatorBase {
public:
    const CANavQuadTreeNode* GetMyNodeAt(const C3DVector& position) const
    {
        return NavigationManager->GetNodeAt(position, PInterface->GetLayerAt(position));
    }
private:
    C3DVector GetPosInDirection(const C3DVector& pos, const C2DVector& direction, float distance) const;
};

C3DVector CNavigatorAStar::GetPosInDirection(const C3DVector& pos, const C2DVector& direction, float distance) const
{
    const float maxDistanceSquared = distance * distance;
    C3DVector current(pos.X, pos.Y, pos.Z);
    C3DVector last(current);
    const CANavQuadTreeNode* node = GetMyNodeAt(pos);
    while (node && !node->Blocked)
    {
        last = current;
        C2DVector step = direction * (node->GetSize() * 0.5f);
        current.X += step.X;
        current.Y += step.Y;
        C2DVector offset(current.X - pos.X, current.Y - pos.Y);
        if (offset.X * offset.X + offset.Y * offset.Y > maxDistanceSquared)
            break;
        node = GetMyNodeAt(current);
    }
    return C3DVector(last.X, last.Y, PInterface->FindPosZAt(last + g_NavHeightTestOffset, true));
}
