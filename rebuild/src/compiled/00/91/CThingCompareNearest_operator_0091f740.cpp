// CThingCompare_Nearest::operator()<T>(T* a, T* b) const -- (T = const CThingObject) return whichever thing is nearer to the reference position: null a loses to b, null b loses to a, else the smaller squared 3D distance wins (a on a strict less, b on a tie)
// Leaf: ??$?R$$CBVCThingObject@@@CThingCompare_Nearest@@QBEPBVCThingObject@@PBV1@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091F740, 177 bytes, thiscall, `ret 8`, the nearer pointer in eax.
// Identity: T = const CThingObject -- proven; evidence: its callers are only the STLport sort family 0x0091F810..0x009204F0 that CAIStateGroup_TestTestTest::Update 0x0091F140 runs (sort 0x009206A0, call at 0x0091F2EF) over the vector filled by GetAllThingsInRoughArea 0x00920200, whose thing-type test is `and edx,807h ; cmp edx,807h` (CThingObject); the PDB name set holds GetAllThingsInRoughArea<const CThingObject, vector<const CThingObject*>, CThingFilter_None> and sort<vector<const CThingObject*>::iterator, CThingCompare_Nearest> (_Insertion_sort<const CThingObject**, CThingCompare_Nearest>), so T = const CThingObject; operator()<CThing> is 0x004A8D80. The FableWin.pdb name set holds this exact
//   symbol (??$?R$$CBVCThingObject@@@CThingCompare_Nearest@@QBEPBVCThingObject@@PBV1@0@Z).
// Label: the manifest's "operator()<CThingMarker>" is propagated across byte-identical bodies (the template body does not depend on T).
// Callees: CThing::GetPosition() const @ 0x004C73D0 (T derives from CThing at offset 0).
// Layout: CThingCompare_Nearest (Ego_r.pdb, 12 bytes) = Pos (C3DVector) +0x00.
// Shape: as the landed CThingCompare_NearestToPos::operator()<CThing> 0x007A2F90: the first delta is spilled to the
//   12-byte frame across the second GetPosition call, the second stays on the x87 stack; both squared lengths are the
//   plain inline X*X + Y*Y + Z*Z (VC7.1 ranks the terms itself); the result is one `?:` on the strict less.
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
class CThingObject : public CThing { };
class CThingCompare_Nearest {
public:
    C3DVector Pos;                                      // +0x00
    template<class T> T* operator()(T* a, T* b) const;
};

template<class T> T* CThingCompare_Nearest::operator()(T* a, T* b) const
{
    if (!a)
        return b;
    if (!b)
        return a;
    C3DVector deltaA = a->GetPosition() - Pos;
    C3DVector deltaB = b->GetPosition() - Pos;
    return (deltaA.GetLengthSquared() < deltaB.GetLengthSquared()) ? a : b;
}

template const CThingObject* CThingCompare_Nearest::operator()<const CThingObject >(const CThingObject* a, const CThingObject* b) const;