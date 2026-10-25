// CDefendingCombatantInfo::CCombatWheel::GetRingRadius(long ring) const -- the outer radius of a combat-wheel ring: the
// sum of the thicknesses of rings 0..ring
// Leaf: ?GetRingRadius@CCombatWheel@CDefendingCombatantInfo@@QBEMJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E8850, `ret 4`, float result on the x87 stack.
// Identity (FINDINGS chunk 388 callee; was the descriptive "SumPrefixWeights_006e8850 (owner uncertain)"): Ego_r.pdb /
//   FableWin.pdb name ?GetRingRadius@CCombatWheel@CDefendingCombatantInfo@@QBEMJ@Z (float (long) const); struct_layouts:
//   CCombatWheel {vector<CCombatRing> CombatRings +0}, CCombatRing {RotationAngle +0, DeltaRotation +4, Index +8,
//   RingThickness +12, vector<CProng> Prongs +16} (28 bytes in retail with the 12-byte STLport vector) = the 28-byte
//   stride and the float at +12 summed here. Callers are all combat-wheel code: ComputeAttackVector 0x006E89B0 (x2),
//   CProng::SetBlocked 0x006E9140, 0x006E94D0 (x2), 0x006E9680, CAttackerPositionEvaluator::EvaluateAttacker 0x006F2C00,
//   CGetBestAttackerForProng::operator() 0x006F2DE0.
// Shape: the inline bounds-checked GetRing(index) (0x006E84E0 out of line elsewhere) inside a countdown loop from `ring`
//   to 0; the null result of an out-of-range index is dereferenced unchecked, as in retail.
class CThingCreatureBase;

template <class T> struct CRetailVector {                   // retail std::vector: {first, last, end_of_storage}
    T* first;
    T* last;
    T* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
};

class CDefendingCombatantInfo {
public:
    class CProng {                                          // 28 bytes
    public:
        bool          Blocked;                              // +0x00
        float         Prong[3];                             // +0x04 C3DVector
        long          ProngBlockageTimer;                   // +0x10
        unsigned char POccupier[8];                         // +0x14 CIntelligentPointer<CThingCreatureBase>
    };
    class CCombatRing {                                     // 28 bytes in retail (PDB 32)
    public:
        float                 RotationAngle;                // +0x00
        float                 DeltaRotation;                // +0x04
        long                  Index;                        // +0x08
        float                 RingThickness;                // +0x0C
        CRetailVector<CProng> Prongs;                       // +0x10
    };
    class CCombatWheel {
    public:
        CRetailVector<CCombatRing> CombatRings;             // +0x00
        const CCombatRing* GetRing(long index) const
        {
            if (index >= 0 && (unsigned long)index < CombatRings.size())
                return &CombatRings.first[index];
            return 0;
        }
        float GetRingRadius(long ring) const;
    };
};

float CDefendingCombatantInfo::CCombatWheel::GetRingRadius(long ring) const
{
    float radius = 0.0f;
    for (long i = ring; i >= 0; --i)
        radius += GetRing(i)->RingThickness;
    return radius;
}
