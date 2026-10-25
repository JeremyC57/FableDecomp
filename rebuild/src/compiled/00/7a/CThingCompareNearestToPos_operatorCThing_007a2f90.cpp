// CThingCompare_NearestToPos::operator()<CThing>(CThing* a, CThing* b) const -- strict-weak "a is nearer to Pos than b":
//   null a never wins, null b always loses, else the squared 3D distances compare
// Leaf: ??$?RVCThing@@@CThingCompare_NearestToPos@@QBE_NPAVCThing@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A2F90, length 167 bytes, `ret 8`, bool in al.
// Identity: the CTCWillLightning TU (retail 0x007A1870 MakeRandomBeam .. 0x007A4080 vfuncs) sorts its candidate CThing*
//   targets with this predicate: 18 callers, all the TU's heap / median / partition / partial-sort helpers (0x007A34A0 is
//   already labelled _Adjust_heap<CThing**,int,CThing*,CThingCompare_NearestToPos>). Ego_r.pdb's CTCWillLightning TU
//   (0x007A7F60 FrameUpdate .. 0x007A92C0) holds exactly ??$?RVCThing@@@CThingCompare_NearestToPos@@QBE_NPAVCThing@@0@Z and
//   its std sort family (_Push_heap / _Adjust_heap / _Make_heap / _Med3 / _Median / _Unguarded_partition / _Sort_heap /
//   _Sort over CThing**). It is not CThingCompare_Nearest: both PDBs give every Nearest::operator()<T> a T* return (the
//   nearer pointer, cf. 0x0091F740), while this body returns a bool in al. FINDINGS chunk 407 (comment vs T* conflict).
// Callees: CThing::GetPosition @ 0x004C73D0.
// Layout: CThingCompare_NearestToPos (Ego_r.pdb, 12 bytes) = Pos (C3DVector) +0x00.
// Shape: the two deltas are `GetPosition() - Pos` through an inline C3DVector operator-; the first is live across
//   the second GetPosition call so it is spilled to the 12-byte frame, the second stays on the x87 stack; both
//   squared lengths are the plain inline `X*X + Y*Y + Z*Z` (VC7.1 ranks the terms itself: Z, X, Y for the spilled
//   delta, Z, Y, X for the stack one; the statement chain forces one order on both and differs); `if (la < lb)
//   return true; return false;` shares the `mov al,1` exit with the null-b test.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

class CThing {
public:
    const C3DVector& GetPosition() const;               // 0x004C73D0
};

class CThingCompare_NearestToPos {
public:
    C3DVector Pos;                                      // +0x00
    template<class T> bool operator()(T* a, T* b) const;
};

template<class T> bool CThingCompare_NearestToPos::operator()(T* a, T* b) const
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

template bool CThingCompare_NearestToPos::operator()<CThing>(CThing* a, CThing* b) const;