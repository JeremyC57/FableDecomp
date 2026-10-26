// CAIStateGroup_JackDragon::FindFlightPathCurvePoint(const C3DVector&, const C3DVector&, const C3DVector&, C3DVector&) -- the curve point for a flight from start to end around a centre: the centre plus the normalised mean of the two centre-relative offsets, scaled by half the start-end distance plus the normalised vector's length
//   left-operand folded forms, `lengthSquared = x * x + (y * y + z * z)` and `r = 0.5f * (r * (3.0f - lengthSquared * (r * r)))`.
//   Same x87 code, more IR before the tail, so VC7.1 hoists the `out` load and `pop esi` above the deferred Z*r as retail.
//   Either form alone moves it 1-2 slots; GetLength's sum must stay left-associated.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094FA00, length 421 bytes (+ int3 padding), `ret 10h` (this unused).
// Globals: 0.5f 0x0122F59C, 3.0f 0x0122DED4, the 128-entry reciprocal-square-root mantissa table 0x013CE558 (C3DVector's
// inline fast Normalise, as landed 0x007A6E20).
// Prototype: FableWin.pdb ?FindFlightPathCurvePoint@CAIStateGroup_JackDragon@@AAEXABVC3DVector@@00AAV2@@Z.
// Shape: three differences built as member operators (x, y, z order), the mean scaled by 0.5f, the landed fast Normalise,
// then centre + dir * (|end - start| * 0.5f + |dir|) in one expression (a separate scale statement reorders the two lengths).
#include <math.h>
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

class C3DVector { public:
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + (y * y + z * z);
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = 0.5f * (r * (3.0f - lengthSquared * (r * r)));
        X *= r;
        Y *= r;
        Z *= r;
    }
};

class CAIStateGroup_JackDragon {
public:
    virtual void v00();
private:
    void FindFlightPathCurvePoint(const C3DVector& start, const C3DVector& end, const C3DVector& centre, C3DVector& out);
};

void CAIStateGroup_JackDragon::FindFlightPathCurvePoint(const C3DVector& start, const C3DVector& end, const C3DVector& centre, C3DVector& out)
{
    C3DVector toStart = start - centre;
    C3DVector toEnd = end - centre;
    C3DVector startToEnd = end - start;
    C3DVector dir = (toStart + toEnd) * 0.5f;
    dir.Normalise();
    out = centre + dir * (startToEnd.GetLength() * 0.5f + dir.GetLength());
}