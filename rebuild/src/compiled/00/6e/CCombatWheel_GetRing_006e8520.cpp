// CCombatWheel::GetRing(long) const -- pointer to the indexed ring, or null when out of range
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E8520, length 57 bytes (+ int3 padding). No callees.
// A negative index fails first (`jl`), then the unsigned compare against size() (the signed /28 reciprocal
// multiply of `last - first`); the element address is `index * 28 + first`. This is the const overload of the byte-identical pair 0x006E84E0 / 0x006E8520.
// Retail std::vector is {first, last, end_of_storage} (12 bytes, no allocator slot; the Ego_r
// PDB layouts carry a 16-byte vector, so every member after one sits 4 bytes lower in retail).
template<class T> struct CRetailVector {
    T* first;
    T* last;
    T* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
};
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                           // +4
    CThing* GetPItem() const;                  // 0x00A01B50
};

struct CProng {                                // CDefendingCombatantInfo::CProng, 28 bytes (PDB)
    bool  Blocked;                             // +0x00
    float Prong_X, Prong_Y, Prong_Z;           // +0x04 C3DVector Prong (PDB +1, packed)
    long  ProngBlockageTimer;                  // +0x10
    CBaseIntelligentPointer POccupier;         // +0x14 CIntelligentPointer<CThingCreatureBase>
};

struct CCombatRing {                           // CDefendingCombatantInfo::CCombatRing, 28 bytes in retail (PDB 32)
    float RotationAngle;                       // +0x00
    float DeltaRotation;                       // +0x04
    long  Index;                               // +0x08
    float RingThickness;                       // +0x0c
    CRetailVector<CProng> Prongs;              // +0x10
    void Reset();                              // 0x006E8B60
};

struct CCombatWheel {                          // CDefendingCombatantInfo::CCombatWheel: one vector of rings
    CRetailVector<CCombatRing> CombatRings;    // +0x00
    const CCombatRing* GetRing(long index) const;
};

const CCombatRing* CCombatWheel::GetRing(long index) const
{
    if (index >= 0 && (unsigned long)index < CombatRings.size())
        return &CombatRings.first[index];
    return 0;
}