// CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator()(const CThing* a, const CThing* b) const -- order two things by how well they lie in the comparator's direction, weighted down with distance (anonymous-namespace comparator)
// Grade: DIFFER(286v286) -- two-call float comparator scheduling: retail loads b (mov ecx,[esp+1Ch]) before the fstp of weight A; ours after
// Leaf: ??RCThingCompare_InDirectionOfAndNearestToHeuristicBitch@
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00978790, 286 bytes, `ret 8` (0x0097ABF0 is a byte-identical copy in another TU).
// Identity: Ego_r.pdb / FableWin.pdb ??RCThingCompare_InDirectionOfAndNearestToHeuristicBitch@?A0x<tu>@@QBE_NPBVCThing@@0@Z
//   (anonymous namespace, one per TU: ?A0xeaff0e08 / ?A0xffab493a; the comparator of a std sort), with the member
//   ?GetWeightFromParams@CThingCompare_InDirectionOfAndNearestToHeuristicBitch@?A0x<tu>@@QBEMMM@Z (expanded here).
// Callees: CThing::GetPos() const @ 0x004C73D0, _ftol2 @ 0x00BFEA70, _CIpow @ 0x00BFEB40 (the `pow` intrinsic). 1.0f 0x0122DED8.
// Layout: PDB CThingCompare_InDirectionOfAndNearestToHeuristicBitch { C2DVector Pos +0; C2DVector Dir +8; } (16 bytes).
// Shape: per thing the 2D offset from Pos, its length (sqrt intrinsic, homed in the dead argument slot), the offset divided by
//   that length (`1.0 / len` then two multiplies), the dot with Dir, and GetWeightFromParams(dot, length) = pow(dot,
//   (long)len / 4 * 2 + 1) (signed divide, `lea [eax+eax+1]`); the first weight is spilled, the second compared on the x87 stack.
extern "C" double __cdecl pow(double, double);
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(pow, sqrt)

class C3DVector { public: float X, Y, Z; };
class C2DVector {
public:
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

class CThing {
public:
    const C3DVector& GetPos() const;            // 0x004C73D0
};

namespace {
class CThingCompare_InDirectionOfAndNearestToHeuristicBitch {
public:
    C2DVector Pos;                              // +0x00
    C2DVector Dir;                              // +0x08
    float GetWeightFromParams(float dot, float distance) const
    {
        return (float)pow(dot, (double)((long)distance / 4 * 2 + 1));
    }
    bool operator()(const CThing* a, const CThing* b) const
    {
        C2DVector offsetA = C2DVector(a->GetPos()) - Pos;
        float distanceA = offsetA.GetLength();
        offsetA /= distanceA;
        float dotA = offsetA * Dir;
        float weightA = GetWeightFromParams(dotA, distanceA);
        C2DVector offsetB = C2DVector(b->GetPos()) - Pos;
        float distanceB = offsetB.GetLength();
        offsetB /= distanceB;
        float dotB = offsetB * Dir;
        if (GetWeightFromParams(dotB, distanceB) < weightA)
            return true;
        return false;
    }
};
}
// Emit the comparator body (retail instantiates it through its std sort; here an explicit out-of-line use).
bool (CThingCompare_InDirectionOfAndNearestToHeuristicBitch::*g_pCompare)(const CThing*, const CThing*) const = &CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator();
