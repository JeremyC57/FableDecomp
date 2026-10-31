// C3DVector::GetRandomPointWithinUnitSphere(unsigned long* seed) -- rejection sampling: random points in the [-1, 1) cube until one lies inside the unit sphere
// Leaf: ?GetRandomPointWithinUnitSphere@C3DVector@@SI?AV1@PAK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A146C0, 278 bytes, plain `ret` (static __fastcall: hidden result in ecx, the
//   seed in edx). Callers 0x00918D15, 0x00B1176D.
// Identity: Ego_r.pdb / FableWin.pdb `?GetRandomPointWithinUnitSphere@C3DVector@@SI?AV1@PAK@Z`, the last vector function
//   in the donor order, right after GetRandomUnitVector (retail 0x00A145C0), as in retail.
// Callees: GFROR13 0x00497890 (Ego_r `?GFROR13@@YIJK@Z`), _CIfmod 0x00BFEB28. Constants: 2^32 0x012387C8 (unsigned fixup),
//   131072.0 0x0129CA60 (2.0f * 65536 folded), 1/65536 0x012387C0, 1.0f 0x0122DED8.
// Shape: GFFloatRandom(2.0f, seed) - 1.0f three times (Ego_r `?GFFloatRandom@@YIMMPAK@Z`), inlined here with the landed
//   out-of-line body of 0x004978A0 (range widened into `scaled`, the LCG step, `scaled *= 65536.0`, GFROR13, the
//   `> 0.0001` test that folds away for the constant 2.0f). The fmod result is a NAMED double local: VC7.1 then schedules the
//   next call's seed update (`imul edi,24A1h ; lea ecx,[edi+24DFh]`) before the `fmul 1/65536`, as retail does; the
//   one-expression `fmod(..) * (1.0 / 65536.0)` puts the fmul first. (The same named-local body still grades
//   RELOCATION_MATCH at 0x004978A0, so one GFFloatRandom body serves both.) X and Y go to the frame, Z stays on the x87
//   stack; the loop repeats while X*X + Y*Y + Z*Z > 1.
#include <math.h>
#pragma intrinsic(fmod)

long __fastcall GFROR13(unsigned long value);                     // 0x00497890

inline float __fastcall GFFloatRandom(float range, unsigned long* seed)   // out of line at 0x004978A0
{
    double scaled = (double)range;
    *seed = *seed * 0x24A1 + 0x24DF;
    scaled *= 65536.0;
    *seed = GFROR13(*seed);
    if (scaled > 0.0001) {
        double result = fmod((double)*seed, scaled);
        return (float)(result * (1.0 / 65536.0));
    }
    return 0.0f;
}

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    static C3DVector __fastcall GetRandomPointWithinUnitSphere(unsigned long* seed);
};

C3DVector __fastcall C3DVector::GetRandomPointWithinUnitSphere(unsigned long* seed)
{
    C3DVector point;
    do {
        point.X = GFFloatRandom(2.0f, seed) - 1.0f;
        point.Y = GFFloatRandom(2.0f, seed) - 1.0f;
        point.Z = GFFloatRandom(2.0f, seed) - 1.0f;
    } while (point.GetLengthSquared() > 1.0f);
    return point;
}