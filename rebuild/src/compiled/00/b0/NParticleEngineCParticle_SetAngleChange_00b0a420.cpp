// NParticleEngine::CParticle::SetAngleChange(float angleChange) -- stores the per-frame spin as a signed byte (angleChange * 63.5, rounded)
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 3-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x00B0A438 `fld dword ptr [esp+4]` + 0x00B0A43C `mov edx,[esp]` + 0x00B0A43F `fistp dword ptr [edx]` -- the engine's pointer-out inline-assembly GFFloatToLong(long* result, float value) helper (the landed retail_asm 0x00B1B5E0 body, 3 instr): the product is stored to the by-value copy at [esp+4] and reloaded, the result pointer copy at [esp] reloaded into edx; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?SetAngleChange@CParticle@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0A420, length 46 bytes, thiscall `ret 4`.
// Signature from FableWin.pdb: ?SetAngleChange@CParticle@NParticleEngine@@QAEXM@Z. Neighbours: SetAngle 0x00B0A410
//   (+0x24), UpdateAngle 0x00B0A450 (`Angle += AngleChange * k`).
// Layout (Ego_r.pdb): CParticle::AngleChange signed char at +0x28. Constant 63.5f @ 0x0129FC10.
// Label: catalog "PackUVForVertexBuffer" is a guess.
// the engine's pointer-out float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline,
// its two by-value parameters get frame copies
__forceinline void GFFloatToLong(long* result, float value)
{
    __asm fld value
    __asm mov edx, result
    __asm fistp dword ptr [edx]
}

namespace NParticleEngine {

class CParticle {
public:
    unsigned char _pad_0x00[0x28];
    signed char   AngleChange;                              // +0x28
    void SetAngleChange(float angleChange);
};

void CParticle::SetAngleChange(float angleChange)
{
    long change;
    GFFloatToLong(&change, angleChange * 63.5f);
    AngleChange = (signed char)change;
}

} // namespace NParticleEngine