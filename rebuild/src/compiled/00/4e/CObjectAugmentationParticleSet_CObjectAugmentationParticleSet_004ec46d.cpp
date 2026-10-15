// CObjectAugmentationParticleSet::CObjectAugmentationParticleSet(const CObjectAugmentationParticleSet& other) -- memberwise copy constructor: the period and the four particle-effect indices, then the start and end point name arrays
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EC46D, length 67 bytes (`ret 4`, eax = this). Callee: the
// CArray<CDefString> copy constructor @ 0x004EC4B0 (manifest "VectorAppendRange"), twice; the twin @ 0x004E4D3B calls another copy of the CArray constructor
// (the inline copy constructor is emitted once per module).
// Layout from the PDB: PeriodSecs +0x00 (float), ParticleEffect +0x04, ParticleEffectToBlendTo +0x08,
// AttackParticleEffect +0x0C, AttackParticleEffectToBlendTo +0x10, StartPoint +0x14 and EndPoint +0x20 (CArray<CDefString>,
// 12 bytes in retail: the PDB's 16-byte arrays put EndPoint at +0x24). Shape: a size-optimised module (#pragma
// optimize("s", on)): every scalar member goes through eax in declaration order (the speed build rotates eax/ecx/edx);
// the last array's source address is formed with `add edi,20h` (the argument register is dead afterwards).
// Leaf: ??0CObjectAugmentationParticleSet
#pragma optimize("s", on)
class CDefString;

class CArrayDefString {                            // CArray<CDefString>, 12 bytes in retail
public:
    CDefString*   m_pData;
    unsigned long m_size;
    unsigned long m_capacity;
    CArrayDefString(const CArrayDefString& other); // 0x004EC4B0
};

class CObjectAugmentationParticleSet {
public:
    float           PeriodSecs;                    // +0x00
    long            ParticleEffect;                // +0x04
    long            ParticleEffectToBlendTo;       // +0x08
    long            AttackParticleEffect;          // +0x0C
    long            AttackParticleEffectToBlendTo; // +0x10
    CArrayDefString StartPoint;                    // +0x14
    CArrayDefString EndPoint;                      // +0x20

    CObjectAugmentationParticleSet(const CObjectAugmentationParticleSet& other);
};

CObjectAugmentationParticleSet::CObjectAugmentationParticleSet(const CObjectAugmentationParticleSet& other)
    : PeriodSecs(other.PeriodSecs),
      ParticleEffect(other.ParticleEffect),
      ParticleEffectToBlendTo(other.ParticleEffectToBlendTo),
      AttackParticleEffect(other.AttackParticleEffect),
      AttackParticleEffectToBlendTo(other.AttackParticleEffectToBlendTo),
      StartPoint(other.StartPoint),
      EndPoint(other.EndPoint)
{
}