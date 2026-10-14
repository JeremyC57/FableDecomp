// CCombatWheel::IsCombatRingOuterRing(const CCombatTypeDef*, unsigned long) -- is the indexed ring at or beyond the def's outer-ring start index
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E84A0, length 60 bytes (+ int3 padding). No callees.
// The ring vector holds 28-byte rings (size() is the signed /28 reciprocal multiply); an index past the
// end is false, otherwise `ring.Index >= def->DefenderWheelOuterRingStartIndex` (PDB +0x50, read at
// +0x48 in retail: two containers before it are 4 bytes smaller) with the ternary giving the bare `setge`.
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
    bool IsCombatRingOuterRing(const struct CCombatTypeDef_Retail* def, unsigned long index);
};

struct CCombatTypeDef_Retail {
    unsigned char _pad[0x48];
    long DefenderWheelOuterRingStartIndex;     // +0x48 (PDB +0x50)
};

bool CCombatWheel::IsCombatRingOuterRing(const CCombatTypeDef_Retail* def, unsigned long index)
{
    if (index < CombatRings.size())
        return CombatRings.first[index].Index >= def->DefenderWheelOuterRingStartIndex ? true : false;
    return false;
}