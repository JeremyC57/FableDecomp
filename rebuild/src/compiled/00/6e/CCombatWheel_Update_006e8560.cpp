// CCombatWheel::Update() -- rotate every ring by its delta and tick down each prong's blockage timer, unblocking at zero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E8560, length 71 bytes. No callees.
// Layout from the Ego_r PDB (CDefendingCombatantInfo::CCombatRing / CProng, retail 12-byte vectors):
// RotationAngle/DeltaRotation at +0/+4, Prongs at +0x10 (28-byte prongs with Blocked at +0 and
// ProngBlockageTimer at +0x10). Both loop bounds are re-read every iteration because the bodies store
// through pointers that may alias them; the inner loop is the natural `if (timer > 0 && --timer == 0)`.
// Retail std::vector is {first, last, end_of_storage} (12 bytes, no allocator slot; the Ego_r
// PDB layouts carry a 16-byte vector, so every member after one sits 4 bytes lower in retail).
template<class T> struct CRetailVector {
    T* first;
    T* last;
    T* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
};
class CThing;

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
    void Update();
};

void CCombatWheel::Update()
{
    for (CCombatRing* ring = CombatRings.first; ring != CombatRings.last; ++ring) {
        ring->RotationAngle += ring->DeltaRotation;
        for (CProng* prong = ring->Prongs.first; prong != ring->Prongs.last; ++prong) {
            if (prong->ProngBlockageTimer > 0) {
                if (--prong->ProngBlockageTimer == 0)
                    prong->Blocked = false;
            }
        }
    }
}