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
#ifndef _MSC_VER
#define __forceinline inline
#endif
#include <math.h>
#include <stdio.h>
unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558 (test: filled in main)

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
    friend struct CTestDragon;
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

// ---- behaviour-test stubs ----
void CAIStateGroup_JackDragon::v00() {}
struct CTestDragon : CAIStateGroup_JackDragon {
    void Run(const C3DVector& s, const C3DVector& e, const C3DVector& c, C3DVector& o) { CallFind(this, s, e, c, o); }
    static void CallFind(CAIStateGroup_JackDragon* d, const C3DVector& s, const C3DVector& e, const C3DVector& c, C3DVector& o);
};
void CTestDragon::CallFind(CAIStateGroup_JackDragon* d, const C3DVector& s, const C3DVector& e, const C3DVector& c, C3DVector& o)
{
    d->FindFlightPathCurvePoint(s, e, c, o);
}
int main()
{
    // a mantissa table consistent with the estimate formula: entry = top 8 mantissa bits of 1/sqrt(x) for the bucket
    for (int i = 0; i < 128; ++i) {
        unsigned long e = 126 + ((i >> 6) & 1);                         // exponent parity from the index
        unsigned long bits = (e << 23) | ((unsigned long)(i & 63) << 17) | (1UL << 16);
        float x = *(float*)&bits;
        long E = (long)((0x17c - e) >> 1);
        double target = 1.0 / sqrt((double)x);
        double mant = target / pow(2.0, (double)(E - 127)) - 1.0;
        long m = (long)(mant * 256.0);
        if (m < 0) m = 0;
        if (m > 255) m = 255;
        g_InvSqrtMantissaTable[i] = (unsigned long)m;
    }
    int ok = 1;
    C3DVector start(10.0f, 0.0f, 0.0f), end(0.0f, 10.0f, 0.0f), centre(0.0f, 0.0f, 0.0f), out(0, 0, 0);
    CTestDragon::CallFind(0, start, end, centre, out);
    // dir = normalise((5,5,0)) = (0.7071, 0.7071, 0); scale = |end-start|*0.5 + 1 = 7.0711 + 1
    float expect = 0.70710678f * (7.0710678f + 1.0f);
    if (fabs(out.X - expect) > 0.05f || fabs(out.Y - expect) > 0.05f || fabs(out.Z) > 0.001f) { ok = 0; printf("out %f %f %f\n", out.X, out.Y, out.Z); }
    C3DVector c2(1.0f, 2.0f, 3.0f), s2(1.0f, 2.0f, 7.0f), e2(1.0f, 2.0f, 5.0f), o2(0, 0, 0);
    CTestDragon::CallFind(0, s2, e2, c2, o2);
    // dir = normalise((0,0,3)) = (0,0,1); scale = 2*0.5 + 1 = 2 -> centre + (0,0,2)
    if (fabs(o2.X - 1.0f) > 0.01f || fabs(o2.Y - 2.0f) > 0.01f || fabs(o2.Z - 5.0f) > 0.05f) { ok = 0; printf("o2 %f %f %f\n", o2.X, o2.Y, o2.Z); }
    if (ok) printf("0094fa00_TEST PASS\n");
    return ok ? 0 : 1;
}