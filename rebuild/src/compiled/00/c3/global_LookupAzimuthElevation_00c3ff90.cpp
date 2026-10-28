// LookupAzimuthElevation(float azimuth, float elevation) -- the libpk audio table entry for a direction: azimuth quantised to 3-degree steps near the horizon (|elevation| < 33), 6-degree steps up to |elevation| < 63 and 12-degree steps above, elevation to 6-degree steps, indexing a 121 x 31 table of unsigned shorts
// Leaf: ?LookupAzimuthElevation@@YAJMM@Z
// Retail-asm: 0x00C40026 `fld dword ptr [esp]` + 0x00C40029 `fistp dword ptr [esp+4]` (and 0x00C40062/0x00C40066, 0x00C4009E/0x00C400A2) -- libpk's inline-assembly float-to-long helper RoundHalfDownToLong (landed 0x00C3FF70, whose own body runs the same `fld`/`fistp dword` pair) expanded in place; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword`, so the bare dword fistp is the original's `__asm`.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3FF90, 318 bytes, __cdecl, long in eax (the table word zero-extended).
// Identity (descriptive; libpk has no PDB/RTTI): the only caller 0x00C4013D (in 0x00C400D0) scales the result by 0x34 into a
//   pair of 52-byte filter records; the table at 0x012B5E10 is read only here. The catalog label
//   "Animation_ComputeFrameOffset" is a propagated guess.
// Callees: RoundHalfDownToLong 0x00C3FF70 (__cdecl; the first call stays out of line, VC7.1 expands the other three -- the
//   same-TU body below is the landed source of that real retail callee).
// Shape: the `>= 63 || < -63` / `>= 33 || < -33` tests are written with the wide band first; VC7.1 lays the 3-degree branch
//   out first (`test ah,1 ; je` / `test ah,5 ; jnp`).
extern const unsigned short g_DirectionFilterTable_012b5e10[];   // 0x012B5E10 (121 x 31 unsigned shorts; descriptive)

long __cdecl RoundHalfDownToLong(float value)   // 0x00C3FF70: same-TU body of the landed retail callee (stand-in, not this row)
{
    long result;
    value -= 0.5f;
    __asm fld value
    __asm fistp result
    if (result < 0)
        ++result;
    return result;
}

long __cdecl LookupAzimuthElevation(float azimuth, float elevation)
{
    long az;
    if (elevation >= 63.0f || elevation < -63.0f)
        az = RoundHalfDownToLong(azimuth - -180.0f + 6.0f) / 12 * 4;
    else if (elevation >= 33.0f || elevation < -33.0f)
        az = RoundHalfDownToLong(azimuth - -180.0f + 3.0f) / 6 * 2;
    else
        az = RoundHalfDownToLong(azimuth - -180.0f + 1.5f) / 3;
    long el = RoundHalfDownToLong(elevation - -90.0f + 3.0f) / 6;
    return g_DirectionFilterTable_012b5e10[az * 31 + el];
}