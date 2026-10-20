// CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator()(CThing* a, CThing* b) const -- order two things by how well they lie along Dir from Pos, the alignment sharpened by distance: pow(cos, (dist/4)*2 + 1)
// Grade: DIFFER(ours v retail) -- 286v286 scheduling only: retail loads the second thing (`mov ecx,[esp+1Ch]`) before spilling the first score and pops esi before fnstsw; ours the other way round
// Leaf: ??RCThingCompare_InDirectionOfAndNearestToHeuristicBitch
// Retail: Fable.exe (TLC, VC7.1) @ 0x00978790, 286 bytes, `ret 8` (0x0097ABF0 is a byte-identical copy in another TU).
// Callees: CThing::GetPosition() @ 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof" is propagated), _ftol2 @ 0x00BFEA70,
//   _CIpow @ 0x00BFEB40 (the `pow` intrinsic). Globals: 1.0f 0x0122DED8.
// Layout: PDB CThingCompare_InDirectionOfAndNearestToHeuristicBitch { C2DVector Pos +0; C2DVector Dir +8; } (16 bytes).
// Shape: an inline per-thing score -- the 2D offset from Pos (a C2DVector local), its length (sqrt intrinsic, homed in the
//   dead argument slot), the offset divided by that length (`1.0 / len` then two multiplies), the dot with Dir, and the
//   odd exponent `(long)len / 4 * 2 + 1` (signed divide, `lea [eax+eax+1]`) fed to pow; the first score is spilled, the
//   second compared on the x87 stack (`a > b` loads b and tests ordered less-than).
extern "C" double __cdecl pow(double, double);
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(pow, sqrt)

struct C3DVector { float X, Y, Z; };
struct C2DVector {
    float X;    // +0
    float Y;    // +4
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    explicit C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) {}
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
    C2DVector& operator/=(float s) { float r = 1.0f / s; X *= r; Y *= r; return *this; }
};
inline C2DVector operator-(const C2DVector& a, const C2DVector& b) { return C2DVector(a.X - b.X, a.Y - b.Y); }
inline float operator*(const C2DVector& a, const C2DVector& b) { return a.X * b.X + a.Y * b.Y; }

struct CThing {
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingCompare_InDirectionOfAndNearestToHeuristicBitch {
    C2DVector Pos;                              // +0x00
    C2DVector Dir;                              // +0x08
    __forceinline float GetHeuristic(CThing* thing) const
    {
        C2DVector offset = C2DVector(thing->GetPosition()) - Pos;
        float distance = offset.GetLength();
        offset /= distance;
        return (float)pow(offset * Dir, (double)((long)distance / 4 * 2 + 1));
    }
    bool operator()(CThing* a, CThing* b) const;
};

bool CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator()(CThing* a, CThing* b) const
{
    float heuristicA = GetHeuristic(a);
    if (GetHeuristic(b) < heuristicA)
        return true;
    return false;
}
