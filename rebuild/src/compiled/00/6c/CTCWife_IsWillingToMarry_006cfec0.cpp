// CTCWife::IsWillingToMarry() const -- a wife-to-be accepts when she is courting and in love with the hero, may marry, holds the wedding ring and a marital home exists
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CFEC0, length 70 bytes (+ int3 padding).
// Callees: CTCWife::IsCourtingHero() const @ 0x006CF890 (twice: directly and inside the inlined IsInLoveWithHero, landed
// out of line at 0x006CF900), CTCWife::FindMaritalHome() const @ 0x006CFAA0 (manifest label).
// Ego_r.pdb: ?IsWillingToMarry@CTCWife@@QBE_NXZ. Layout: engine/CTCWife.h OpinionAttitude +0x10; ReceivedWeddingRing is
// retail +0x91 (header +0x95: the retail object sits 4 bytes below the PDB past the GiftsReceived map). The owner thing's
// flag byte +0x1c9 bit 4 must be set.
// Shape: `if (a && b && c && d && e) return true; return false;` -- shared `mov al,1` / `xor al,al` tails.
class CThing { public:
    unsigned char _pad_00[0x1c9];
    unsigned char m_flags1c9;                               // +0x1c9 (bit 4: may marry)
};

enum EOpinionAttitudeType { OPINION_ATTITUDE_LOVE = 12 };

extern const float g_zero;                                  // 0x0122DEDC (0.0f)

class CThingBuilding;

class CTCWife { public:                                            // retail layout: engine/CTCWife.h up to the GiftsReceived map,
    void*                __vftable;                         // +0x00   4 bytes below the PDB after it (12-byte retail map)
    CThing*              ParentThing;                       // +0x04
    bool                 ToBeRemoved;                       // +0x08
    bool                 IsInGlobalUpdateTCs;               // +0x09
    void*                PHeroMarriageDef;                  // +0x0c CDefPointer<CHeroMarriageDef const>
    EOpinionAttitudeType OpinionAttitude;                   // +0x10
    unsigned char        _pad_0x14[0x8c - 0x14];
    float                LoveAttitudeValue;                 // +0x8c (PDB +0x90)
    bool                 HasBeenInLoveWithPlayer;           // +0x90 (PDB +0x94)
    bool                 ReceivedWeddingRing;               // +0x91 (PDB +0x95)
    bool IsCourtingHero() const;                            // 0x006CF890
    bool IsInLoveWithHero() const { return IsCourtingHero() && OpinionAttitude == OPINION_ATTITUDE_LOVE; }   // 0x006CF900 out of line
    CThingBuilding* FindMaritalHome() const;                // 0x006CFAA0
    bool IsWillingToMarry() const;
};

bool CTCWife::IsWillingToMarry() const
{
    if (IsCourtingHero() && IsInLoveWithHero() && (ParentThing->m_flags1c9 & 0x10) && ReceivedWeddingRing && FindMaritalHome())
        return true;
    return false;
}