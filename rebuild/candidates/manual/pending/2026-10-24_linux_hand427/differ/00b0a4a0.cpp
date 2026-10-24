// NParticleEngine::CParticle::SetCreationTime(float creationTime) -- stores the particle's creation time as a byte fraction (creationTime * 255, rounded)
// Grade: DIFFER(45v46) -- retail_asm candidate: the rounding is the inline-asm GFFloatToLong(float, long*) helper (`lea eax,[esp+0Ch] ; ... ; mov edx,[esp] ; fistp dword ptr [edx]`, the result written over the dead argument slot), which VC7.1 cannot emit from C++; this is the C++ reference body.
// Leaf: ?SetCreationTime@CParticle@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0A4A0, length 46 bytes, thiscall `ret 4`.
// Signature from FableWin.pdb: ?SetCreationTime@CParticle@NParticleEngine@@QAEXM@Z (GetCreationTime returns the float).
// Layout (Ego_r.pdb): CParticle::CreationTime unsigned char at +0x74. Constant 255.0f @ 0x01230014.
// Label: catalog "PackUVForVertexBuffer" is a guess (and duplicates 0x00B0A420's).
#include <math.h>

inline long GFFloatToLong(float f) { return (long)floor(f + 0.5f); }   // C++ stand-in for the inline-asm fld/fistp

namespace NParticleEngine {

class CParticle {
public:
    unsigned char _pad_0x00[0x74];
    unsigned char CreationTime;                             // +0x74
    void SetCreationTime(float creationTime);
};

void CParticle::SetCreationTime(float creationTime)
{
    CreationTime = (unsigned char)GFFloatToLong(creationTime * 255.0f);
}

} // namespace NParticleEngine
