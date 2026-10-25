// NParticleEngine::GFInterpolate(long a, long b, float t) -- integer linear interpolation a * (1 - t) + b * t, rounded to the nearest long
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x00B05C41 `fstp [esp+0Ch]` / 0x00B05C45 `fld dword ptr [esp+0Ch]` + 0x00B05C49 `fistp dword ptr [esp+4]` -- the engine's inline-assembly GFFloatToLong helper (by-value copy stored and reloaded, bare dword fistp into a frame word). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?GFInterpolate@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B05C20, length 55 bytes, __fastcall (a in ecx, b in edx, both spilled
//   for `fimul` / `fild`), `ret 4`.
// Signature from FableWin.pdb: ?GFInterpolate@NParticleEngine@@YIJJJM@Z (the long overload of the family). No direct
//   caller in retail (the CPSC InterpolateTo bodies inline it).
// Constants: 1.0f @ 0x0122DED8.
// Label: catalog "GFInterpolate" (_global) -- the namespace is NParticleEngine.
// the engine's inline-assembly float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long GFFloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

namespace NParticleEngine {

long __fastcall GFInterpolate(long a, long b, float t)
{
    return GFFloatToLong((1.0f - t) * a + b * t);
}

} // namespace NParticleEngine