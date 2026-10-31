// CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator()(const CThing* a, const CThing* b) const -- order two things by how well they lie along Dir from Pos, the alignment sharpened by distance: pow(cos, (dist/4)*2 + 1)
// Grade: DIFFER(286v286) -- same two scheduling moves as twin 0x00978790 (second-thing load v first-score fstp; pop esi v fnstsw); the twin's PDB respell and real C2DVector API regraded here (same)
// Leaf: ??RCThingCompare_InDirectionOfAndNearestToHeuristicBitch
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097ABF0, 286 bytes (+2 int3), `ret 8` (a byte-identical copy of 0x00978790 in another TU: its own anonymous namespace).
// Callees: CThing::GetPos() const @ 0x004C73D0 (Ego_r.pdb ?GetPos@CThing@@QBEABVC3DVector@@XZ; manifest "CVertexBufferWin32::DoSizeof" is propagated), _ftol2 @ 0x00BFEA70,
//   _CIpow @ 0x00BFEB40 (the `pow` intrinsic). Globals: 1.0f 0x0122DED8.
// Layout: PDB CThingCompare_InDirectionOfAndNearestToHeuristicBitch { C2DVector Pos +0; C2DVector Dir +8; } (16 bytes), in an
//   anonymous namespace per TU (Ego_r.pdb ??RCThingCompare_InDirectionOfAndNearestToHeuristicBitch@?A0xeaff0e08@@QBE_NPBVCThing@@0@Z and
//   the ?A0xffab493a twin): the two retail bodies are the two TUs' copies, distinct symbols by namespace.
// Shape: an inline per-thing score -- the 2D offset from Pos (a C2DVector local), its length (sqrt intrinsic, homed in the
//   dead argument slot), the offset divided by that length (`1.0 / len` then two multiplies), the dot with Dir, and the
//   odd exponent `(long)len / 4 * 2 + 1` (signed divide, `lea [eax+eax+1]`) fed to pow; the first score is spilled, the
//   second compared on the x87 stack (`a > b` loads b and tests ordered less-than).
extern "C" double __cdecl pow(double, double);
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(pow, sqrt)

class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X;    // +0
    float Y;    // +4
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) {}
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y); }
    const C2DVector& operator/=(float s) { float r = 1.0f / s; X *= r; Y *= r; return *this; }
    float Dot(const C2DVector& v) const { return X * v.X + Y * v.Y; }
};

class CThing { public:
    const C3DVector& GetPos() const;            // 0x004C73D0
};
namespace {
class CThingCompare_InDirectionOfAndNearestToHeuristicBitch { public:
    C2DVector Pos;                              // +0x00
    C2DVector Dir;                              // +0x08
    __forceinline float GetHeuristic(const CThing* thing) const
    {
        C2DVector offset = C2DVector(thing->GetPos()) - Pos;
        float distance = offset.GetMagnitude();
        offset /= distance;
        return (float)pow(offset.Dot(Dir), (double)((long)distance / 4 * 2 + 1));
    }
    bool operator()(const CThing* a, const CThing* b) const;
};
}

bool CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator()(const CThing* a, const CThing* b) const
{
    float heuristicA = GetHeuristic(a);
    if (GetHeuristic(b) < heuristicA)
        return true;
    return false;
}
