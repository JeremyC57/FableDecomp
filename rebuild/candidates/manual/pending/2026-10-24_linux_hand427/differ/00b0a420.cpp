// NParticleEngine::CParticle::SetAngleChange(float angleChange) -- stores the per-frame spin as a signed byte (angleChange * 63.5, rounded)
// Grade: DIFFER(45v46) -- retail_asm candidate: the rounding is the inline-asm GFFloatToLong(float, long*) helper (`lea eax,[esp+0Ch] ; ... ; mov edx,[esp] ; fistp dword ptr [edx]`, the result written over the dead argument slot), which VC7.1 cannot emit from C++; this is the C++ reference body.
// Leaf: ?SetAngleChange@CParticle@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0A420, length 46 bytes, thiscall `ret 4`.
// Signature from FableWin.pdb: ?SetAngleChange@CParticle@NParticleEngine@@QAEXM@Z. Neighbours: SetAngle 0x00B0A410
//   (+0x24), UpdateAngle 0x00B0A450 (`Angle += AngleChange * k`).
// Layout (Ego_r.pdb): CParticle::AngleChange signed char at +0x28. Constant 63.5f @ 0x0129FC10.
// Label: catalog "PackUVForVertexBuffer" is a guess.
#include <math.h>

inline long GFFloatToLong(float f) { return (long)floor(f + 0.5f); }   // C++ stand-in for the inline-asm fld/fistp

namespace NParticleEngine {

class CParticle {
public:
    unsigned char _pad_0x00[0x28];
    signed char   AngleChange;                              // +0x28
    void SetAngleChange(float angleChange);
};

void CParticle::SetAngleChange(float angleChange)
{
    AngleChange = (signed char)GFFloatToLong(angleChange * 63.5f);
}

} // namespace NParticleEngine
