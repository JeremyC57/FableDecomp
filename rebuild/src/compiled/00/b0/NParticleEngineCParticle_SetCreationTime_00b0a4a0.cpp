// NParticleEngine::CParticle::SetCreationTime(float creationTime) -- stores the particle's creation time as a byte fraction (creationTime * 255, rounded)
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 3-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x00B0A4B8 `fld dword ptr [esp+4]` + 0x00B0A4BC `mov edx,[esp]` + 0x00B0A4BF `fistp dword ptr [edx]` -- the engine's pointer-out inline-assembly GFFloatToLong(long* result, float value) helper (the landed retail_asm 0x00B1B5E0 body, 3 instr): the product is stored to the by-value copy at [esp+4] and reloaded, the result pointer copy at [esp] reloaded into edx; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?SetCreationTime@CParticle@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0A4A0, length 46 bytes, thiscall `ret 4`.
// Signature from FableWin.pdb: ?SetCreationTime@CParticle@NParticleEngine@@QAEXM@Z (GetCreationTime returns the float).
// Layout (Ego_r.pdb): CParticle::CreationTime unsigned char at +0x74. Constant 255.0f @ 0x01230014.
// Label: catalog "PackUVForVertexBuffer" is a guess (and duplicates 0x00B0A420's).
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
    unsigned char _pad_0x00[0x74];
    unsigned char CreationTime;                             // +0x74
    void SetCreationTime(float creationTime);
};

void CParticle::SetCreationTime(float creationTime)
{
    long time;
    GFFloatToLong(&time, creationTime * 255.0f);
    CreationTime = (unsigned char)time;
}

} // namespace NParticleEngine