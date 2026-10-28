// CThingCompare_Nearest::operator()<T>(T* a, T* b) const -- (T = CThingCreatureBase) return whichever thing is nearer to the reference position: null a loses to b, null b loses to a, else the smaller squared 3D distance wins (a on a strict less, b on a tie)
// Leaf: ??$?RVCThingCreatureBase@@@CThingCompare_Nearest@@QBEPAVCThingCreatureBase@@PAV1@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00782560, 177 bytes, thiscall, `ret 8`, the nearer pointer in eax.
// Identity: T = CThingCreatureBase -- proven; evidence: its callers are the landed GetPBestThing<CThingFilter_HasTC, CThingCompare_Nearest, CThingCreatureBase> 0x00782670 and the GetPBestThingInRoughArea<.., CThingCompare_Nearest, CThingCreatureBase> searches 0x0079A050 / 0x009147E0 / 0x00937420 / 0x00937690 / 0x00942390 (mask 0xF); operator()<const CThingCreatureBase> is G015's 0x006EF290. The FableWin.pdb name set holds this exact
//   symbol (??$?RVCThingCreatureBase@@@CThingCompare_Nearest@@QBEPAVCThingCreatureBase@@PAV1@0@Z).
// Label: the manifest's "operator()<CThingMarker>" is propagated across byte-identical bodies (the template body does not depend on T).
// Callees: CThing::GetPos() const @ 0x004C73D0 (T derives from CThing at offset 0).
// Layout: CThingCompare_Nearest (Ego_r.pdb, 12 bytes) = Pos (C3DVector) +0x00.
// Shape: as the landed CThingCompare_NearestToPos::operator()<CThing> 0x007A2F90: the first delta is spilled to the
//   12-byte frame across the second GetPos call, the second stays on the x87 stack; both squared lengths are the
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
    const C3DVector& GetPos() const;               // 0x004C73D0
};
class CThingCreatureBase : public CThing { };
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
    C3DVector deltaA = a->GetPos() - Pos;
    C3DVector deltaB = b->GetPos() - Pos;
    return (deltaA.GetLengthSquared() < deltaB.GetLengthSquared()) ? a : b;
}

template CThingCreatureBase* CThingCompare_Nearest::operator()<CThingCreatureBase >(CThingCreatureBase* a, CThingCreatureBase* b) const;