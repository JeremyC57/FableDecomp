// GFInitMaths() -- build the fast-maths lookup tables: the 128-entry reciprocal-square-root mantissa table (entry 64 forced to 0xFF), 1024 cosines over a full turn (entry 1024 repeats entry 0), and 1025 acos / asin samples over [-1, 1] (each with a zero guard entry)
// Leaf: ?GFInitMaths@@
// Grade: RELOCATION_MATCH -- union in/out for the bit-pattern float round trip gives retail's two slots (in [esp], out
//   and the fild temporaries [esp+4])
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0DB60, 290 bytes, __fastcall with no arguments, plain `ret`. Callers: the
//   system manager start-up (0x009A5480, 0x009A69A8).
// Identity: Ego_r.pdb `?GFInitMaths@@YIXXZ` (the chunk label "Math_InitializeCosineLookup" is descriptive).
// Globals: the reciprocal-square-root mantissa table 0x013CE558 (128 dwords, as the landed fast-inverse-sqrt readers name it),
//   the cosine table 0x013CD550, the acos table 0x013CB530 and the asin table 0x013CC540 (1025 samples + a guard each).
// Callees: _CIacos (thunk 0x00BFEBA2), _CIasin (thunk 0x00BFEDC4); sqrt / cos are x87 intrinsics.
// Constants: 1.0 (double) 0x01237F00, 2^32 (unsigned-to-float fix) 0x0122DCB4, pi/1024 0x0129CA40, 2/1025 0x0129CA3C,
//   1.0f 0x0122DED8.
// Shape: the mantissa loop builds each sample float from its bit pattern ((i | 0x1F80) << 17: [0.5, 2)) and reads the
//   rounded result back as bits; the table loops convert their unsigned counters to float (the `fadd 2^32` fix-up).
typedef unsigned short wchar_t;                     // VC7.1 default /Zc:wchar_t-
extern "C" double __cdecl sqrt(double x);
extern "C" double __cdecl cos(double x);
extern "C" double __cdecl acos(double x);
extern "C" double __cdecl asin(double x);
#pragma intrinsic(sqrt, cos, acos, asin)

extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
extern float g_CosTable[1026];                      // 0x013CD550
extern float g_ACosTable[1028];                     // 0x013CB530
extern float g_ASinTable[1028];                     // 0x013CC540

void __fastcall GFInitMaths()
{
    unsigned long i;
    union { unsigned long I; float F; } in, out;
    unsigned long* entry = g_InvSqrtMantissaTable;
    for (i = 0; i < 128; ++i) {
        in.I = (i | 0x1F80) << 17;
        out.F = (float)(1.0 / sqrt(in.F));
        *entry++ = ((out.I + 0x2000) >> 15) & 0xFF;
    }
    g_InvSqrtMantissaTable[64] = 0xFF;

    for (i = 0; i < 1024; ++i)
        g_CosTable[i] = (float)cos((float)(i * 2) * 0.0030679617f);
    g_CosTable[1024] = g_CosTable[0];

    for (i = 0; i < 1025; ++i)
        g_ACosTable[i] = (float)acos((float)i * 0.0019512195f - 1.0f);
    g_ACosTable[1025] = 0.0f;

    for (i = 0; i < 1025; ++i)
        g_ASinTable[i] = (float)asin((float)i * 0.0019512195f - 1.0f);
    g_ASinTable[1025] = 0.0f;
}