// NParticleEngine::CParticle::GetRecordedPos(float time, float offset) const -- position along the particle's 4-entry recorded-position buffer at time - offset (+1), interpolated linearly between the two bracketing entries
// Leaf: ?GetRecordedPos@CParticle@NParticleEngine@@QBE?AVC3DVector@@MM@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AFE380, length 178 bytes, thiscall `ret 0Ch` (hidden result pointer,
//   time, offset), ebp frame.
// Signature from FableWin.pdb: ?GetRecordedPos@CParticle@NParticleEngine@@QBE?AVC3DVector@@MM@Z. No direct caller in
//   retail.
// Layout (Ego_r.pdb, retail identical here): CParticle::PositionBuffer C3DVector[4] at +0x29 (1-aligned C3DVector).
// Shape: pos = time + 1 - offset; index = ceil(pos) clamped to [1, 3]; frac = index - pos; result = Buffer[index] +
//   (Buffer[index - 1] - Buffer[index]) * frac.  The 0.5f and 1.0f constants are stored to frame words (the 1.0f into
//   the dead `offset` slot), the inline-asm operand convention.
// Label: catalog "Audio_InterpolateFilterCoefficient" is a guess.
// Retail-asm: 0x00AFE3A6..0x00AFE3C0 `fld [ebp-8] ; fadd [ebp-4] ; fistp dword ptr [ebp+0Ch] ; fild [ebp+0Ch] ; fsub [ebp+10h] ; fstp [ebp-0Ch] ; mov eax,[ebp-8] ; cmp eax,[ebp-0Ch] ; jne ; dec dword ptr [ebp+0Ch]` (1.0f and the result homed in the dead `offset` / `time` argument slots) -- the engine's inline-assembly ceiling helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; dec dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongCeil.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
#pragma optimize("y", off)

#include "engine/C3DVector.h"

// the engine's inline-assembly FPU ceiling helper (0x004FA9A0 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory decrement included: rasm1 family (b)); the result
// is read back by C++ after the block.
#include "engine/GFFloatToLongCeil.h"

namespace NParticleEngine {

#pragma pack(push, 1)
class CParticle {
public:
    unsigned char _pad_0x00[0x29];
    C3DVector     PositionBuffer[4];                        // +0x29
    C3DVector GetRecordedPos(float time, float offset) const;
};
#pragma pack(pop)

C3DVector CParticle::GetRecordedPos(float time, float offset) const
{
    float pos = time + 1.0f - offset;
    long index = GFFloatToLongCeil(pos);
    if (index < 1)
        index = 1;
    else if (index > 3)
        index = 3;
    float frac = (float)index - pos;
    const C3DVector& newer = PositionBuffer[index - 1];
    const C3DVector& older = PositionBuffer[index];
    return older + (newer - older) * frac;
}

} // namespace NParticleEngine