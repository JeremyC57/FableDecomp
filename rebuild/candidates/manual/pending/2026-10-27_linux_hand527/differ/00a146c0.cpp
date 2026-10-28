// C3DVector::GetRandomPointWithinUnitSphere(unsigned long* seed) -- rejection sampling: random points in the [-1, 1) cube until one lies inside the unit sphere
// Grade: DIFFER(278v278) -- scheduling only: after each inlined GFFloatRandom's _CIfmod call retail runs the next seed update (imul edi,24A1h ; lea ecx,[edi+24DFh]) before the fmul 1/65536; ours does the fmul first. Division/multiply, statement order, locals, for/do loop, /G5-/G7 inert.
// Leaf: ?GetRandomPointWithinUnitSphere@C3DVector@@SI?AV1@PAK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A146C0, 278 bytes, plain `ret` (static __fastcall: hidden result in ecx, the
//   seed in edx). Callers 0x00918D15, 0x00B1176D.
// Identity: Ego_r.pdb / FableWin.pdb `?GetRandomPointWithinUnitSphere@C3DVector@@SI?AV1@PAK@Z`, the last vector function
//   in the donor order, right after GetRandomUnitVector (retail 0x00A145C0), as in retail.
// Callees: GFROR13 0x00497890 (`?GFROR13@@YIJK@Z`), _CIfmod 0x00BFEB28. Constants: 2^32 0x012387C8 (unsigned fixup),
//   131072.0 0x0129CA60 (2.0f * 65536 folded), 1/65536 0x012387C0, 1.0f 0x0122DED8.
// Shape: GFFloatRandom(2.0f, seed) - 1.0f three times, inlined here (the out-of-line copy 0x004978A0 is the same
//   body: range * 65536, seed = GFROR13(seed * 9377 + 9439), 0 when the range is not above 0.0001, else
//   fmod(seed, range) / 65536; the range test folds away for the constant 2.0f). X and Y go to the frame, Z stays on
//   the x87 stack; the loop repeats while Z*Z + Y*Y + X*X > 1.
#include <math.h>
#pragma intrinsic(fmod)

long __fastcall GFROR13(unsigned long value);                     // 0x00497890

inline float __fastcall GFFloatRandom(float range, unsigned long* seed)   // out of line at 0x004978A0
{
    double scaled = range * 65536.0;
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    if (scaled <= 0.0001)
        return 0.0f;
    return (float)(fmod((double)*seed, scaled) * (1.0 / 65536.0));
}

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    float GetSquaredMagnitude() const { return X * X + Y * Y + Z * Z; }
    static C3DVector __fastcall GetRandomPointWithinUnitSphere(unsigned long* seed);
};

C3DVector __fastcall C3DVector::GetRandomPointWithinUnitSphere(unsigned long* seed)
{
    C3DVector point;
    do {
        point.X = GFFloatRandom(2.0f, seed) - 1.0f;
        point.Y = GFFloatRandom(2.0f, seed) - 1.0f;
        point.Z = GFFloatRandom(2.0f, seed) - 1.0f;
    } while (point.GetSquaredMagnitude() > 1.0f);
    return point;
}
