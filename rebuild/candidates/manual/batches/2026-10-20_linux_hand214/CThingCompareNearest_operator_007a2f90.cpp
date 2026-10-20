// CThingCompare_Nearest::operator()<CThingAICreature>(CThingAICreature* a, CThingAICreature* b) const -- strict-weak "a is nearer to Pos than b": null a never wins, null b always loses, else the squared 3D distances compare
// Leaf: ??$?RVCThingAICreature@@@CThingCompare_Nearest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A2F90, length 167 bytes, `ret 8`, bool in al.
// Callees: CThing::GetPosition @ 0x004C73D0.
// Layout: CThingCompare_Nearest (Ego_r.pdb, 12 bytes) = Pos (C3DVector) +0x00.
// Shape: the two deltas are `GetPosition() - Pos` through an inline C3DVector operator-; the first is live across
//   the second GetPosition call so it is spilled to the 12-byte frame, the second stays on the x87 stack; both
//   squared lengths are the plain inline `X*X + Y*Y + Z*Z` (VC7.1 ranks the terms itself: Z, X, Y for the spilled
//   delta, Z, Y, X for the stack one; the statement chain forces one order on both and differs); `if (la < lb)
//   return true; return false;` shares the `mov al,1` exit with the null-b test.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

struct CThing {
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
struct CThingCreatureBase : CThing { };
struct CThingAICreature : CThingCreatureBase { };

struct CThingCompare_Nearest {
    C3DVector Pos;                                      // +0x00
    template<class T> bool operator()(T* a, T* b) const;
};

template<class T> bool CThingCompare_Nearest::operator()(T* a, T* b) const
{
    if (!a)
        return false;
    if (!b)
        return true;
    C3DVector deltaA = a->GetPosition() - Pos;
    C3DVector deltaB = b->GetPosition() - Pos;
    if (deltaA.GetLengthSquared() < deltaB.GetLengthSquared())
        return true;
    return false;
}

template bool CThingCompare_Nearest::operator()<CThingAICreature>(CThingAICreature* a, CThingAICreature* b) const;
