// CCombatWheel::ResetRings() -- reset every ring of the wheel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E8BD0, length 36 bytes.
// Callees: CCombatRing::Reset @ 0x006E8B60 on each 28-byte ring.
// The vector's `last` is re-read after every call; the `jmp` into the aligned loop head is VC7.1's loop
// layout. This is the one row of the five "ResetRings"-labelled bodies in the chunk that really is this
// function (0x006E8xxx is the CCombatWheel module; the other four are propagated vector shapes).
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
    void ResetRings();
};

void CCombatWheel::ResetRings()
{
    for (CCombatRing* ring = CombatRings.first; ring != CombatRings.last; ++ring)
        ring->Reset();
}