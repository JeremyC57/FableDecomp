// CCombatRing::GetAttackerProng(CThing*) -- the prong occupied by the given attacker, or null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E87D0, length 55 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (on each prong's POccupier at +0x14).
// Layout from the Ego_r PDB (CDefendingCombatantInfo::CCombatRing / CProng): Prongs is a std::vector
// of 28-byte prongs at +0x10 (retail 12-byte vector), each with the occupier intelligent pointer at
// +0x14. The vector's `last` is re-read after every call (the out-of-line GetPItem may alias it).
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
    CProng* GetAttackerProng(CThing* attacker);
};

CProng* CCombatRing::GetAttackerProng(CThing* attacker)
{
    for (CProng* prong = Prongs.first; prong != Prongs.last; ++prong) {
        if (prong->POccupier.GetPItem() == attacker)
            return prong;
    }
    return 0;
}
