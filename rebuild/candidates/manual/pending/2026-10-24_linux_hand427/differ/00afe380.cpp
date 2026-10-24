// NParticleEngine::CParticle::GetRecordedPos(float time, float offset) const -- position along the particle's 4-entry recorded-position buffer at time - offset (+1), interpolated linearly between the two bracketing entries
// Grade: DIFFER(161v178) -- retail_asm candidate: the ceiling of the buffer position is an inline-asm GFFloatToLongCeil (`fadd 0.5 ; fistp dword ptr [ebp+0Ch]` plus an integer compare of the float bits for the exact-integer case), which also gives the function its ebp frame; this is the C++ reference body.
// Leaf: ?GetRecordedPos@CParticle@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AFE380, length 178 bytes, thiscall `ret 0Ch` (hidden result pointer,
//   time, offset), ebp frame.
// Signature from FableWin.pdb: ?GetRecordedPos@CParticle@NParticleEngine@@QBE?AVC3DVector@@MM@Z. No direct caller in
//   retail.
// Layout (Ego_r.pdb, retail identical here): CParticle::PositionBuffer C3DVector[4] at +0x29 (1-aligned C3DVector).
// Shape: pos = time + 1 - offset; index = ceil(pos) clamped to [1, 3]; frac = index - pos; result = Buffer[index] +
//   (Buffer[index - 1] - Buffer[index]) * frac.  The 0.5f and 1.0f constants are stored to frame words (the 1.0f into
//   the dead `offset` slot), the inline-asm operand convention.
// Label: catalog "Audio_InterpolateFilterCoefficient" is a guess.
#include <math.h>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};

inline long GFFloatToLongCeil(float f) { return (long)ceil(f); }   // C++ stand-in for the inline-asm ceiling

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
