// CThingCompare_Nearest::operator()<T>(T* a, T* b) const -- (T = const CThingAICreature) return whichever thing is nearer to the reference position: null a loses to b, null b loses to a, else the smaller squared 3D distance wins (a on a strict less, b on a tie)
// Leaf: ??$?R$$CBVCThingAICreature@@@CThingCompare_Nearest@@QBEPBVCThingAICreature@@PBV1@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00986C30, 177 bytes, thiscall, `ret 8`, the nearer pointer in eax.
// Identity: T = const CThingAICreature -- proven; evidence: its only caller is the landed GetPBestThing<CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsAlliedWithFaction>, CThingCompare_Nearest, const CThingAICreature> 0x00986D90 (PDB name set: ??$GetPBestThing@...@VCThingCompare_Nearest@@$$CBVCThingAICreature@@...). The FableWin.pdb name set holds this exact
//   symbol (??$?R$$CBVCThingAICreature@@@CThingCompare_Nearest@@QBEPBVCThingAICreature@@PBV1@0@Z).
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
class CThingAICreature : public CThing { };
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

template const CThingAICreature* CThingCompare_Nearest::operator()<const CThingAICreature >(const CThingAICreature* a, const CThingAICreature* b) const;