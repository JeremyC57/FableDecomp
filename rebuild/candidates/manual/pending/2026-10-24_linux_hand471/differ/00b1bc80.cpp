// NParticleEngine::CPSCDecalRenderer::GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float unused, float time) -- with a decal texture set: take the larger of the two packed decal dimensions (each an 11-bit fraction of 20 units, rounded to two decimals), scale it by the particle system, and add twice the time to the cost's first term and size squared times time plus twice the time to its fourth
// Grade: DIFFER(261v280) -- retail_asm candidate: the two-decimal rounding is a bare `fld ; fistp dword [esp] ; fild` (retail inline asm, as in the sibling rows of this TU); VC7.1 C++ casts call __ftol2. Reference body only, no grade beyond one.
// Leaf: ?GetComponentCost@CPSCDecalRenderer@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1BC80, 280 bytes (+ int3 padding), `ret 10h`. Slot 20 of the CPSCDecalRenderer
//   vtable 0x012A09BC; Ego_r.pdb ?GetComponentCost@CPSCDecalRenderer@NParticleEngine@@UAEXAAVCParticleSystemCost@2@
//   ABVCParticleSystem@2@MM@Z (the only four-argument void member of the class). Callees: CParticleSystem::
//   ScaleParticleDimensions(float*) @ 0x00AF8F80. Floats: 1/2047 @ 0x0129F65C, 20.0 @ 0x0122F160, 1.0 @ 0x0122DED8,
//   10.0 @ 0x01237F0C (the inline power-of-ten loop), 2^32 @ 0x0122DCB4 (unsigned fix-up).
// Layout (descriptive): texture handle +0x10, packed dimension bits 10..20 of +0x24 and bits 0..10 of +0x28;
//   CParticleSystemCost terms +0x00 and +0x0C.
#include <stddef.h>
struct CParticleSystemCost { float Term0; float Term1; float Term2; float Term3; };
class CParticleSystem {
public:
    void ScaleParticleDimensions(float* size) const;    // 0x00AF8F80
};
template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }   // STLport max
inline float RoundToDecimalPlaces(float value, long places)
{
    float scale = 1.0f;
    for (long i = places; i; --i)
        scale *= 10.0f;
    long rounded = (long)(value * scale);           // retail: inline-asm fistp (round to nearest)
    return (float)rounded / scale;
}
namespace NParticleEngine {
class CPSCDecalRenderer {
public:
    virtual void GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float unused, float time);
    unsigned char _pad04[0x10 - 4];
    void*         m_texture;                        // +0x10
    unsigned char _pad14[0x24 - 0x14];
    unsigned long : 10;
    unsigned long m_width : 11;                     // +0x24 bits 10..20
    unsigned long : 11;
    unsigned long m_height : 11;                    // +0x28 bits 0..10
};

void CPSCDecalRenderer::GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float unused, float time)
{
    if (!m_texture)
        return;
    float width = RoundToDecimalPlaces((float)m_width * (1.0f / 2047.0f) * 20.0f, 2);
    float size = RoundToDecimalPlaces((float)m_height * (1.0f / 2047.0f) * 20.0f, 2);
    size = Max(width, size);
    system.ScaleParticleDimensions(&size);
    float twice = time + time;
    cost.Term0 += twice;
    cost.Term3 = size * size * time + cost.Term3 + twice;
}
}
