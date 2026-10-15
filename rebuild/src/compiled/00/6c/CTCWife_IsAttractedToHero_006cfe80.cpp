// CTCWife::IsAttractedToHero() const -- a courting wife is attracted while her love attitude is positive or once she is in love with the hero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CFE80, length 59 bytes (+ int3 padding).
// Callees: CTCWife::IsCourtingHero() const @ 0x006CF890 (twice: once directly, once inside the inlined IsInLoveWithHero,
// whose out-of-line copy is the landed 0x006CF900). Float 0x0122DEDC = 0.0f.
// Ego_r.pdb: ?IsAttractedToHero@CTCWife@@QBE_NXZ. Layout: engine/CTCWife.h OpinionAttitude +0x10 (12 = love); past the
// GiftsReceived map the retail object sits 4 bytes below the PDB, so LoveAttitudeValue is +0x8c (header +0x90) -- the
// local model carries the retail offsets.
// Shape: `a && (b || c)` with a bool result: `fcomp ; test ah,41h ; je` jumps to the shared `mov eax,1` for `> 0.0f`,
// full-register `mov eax,1` / `xor eax,eax` tails.
struct CThing {
    unsigned char _pad_00[0x1c9];
    unsigned char m_flags1c9;                               // +0x1c9 (bit 4: may marry)
};

enum EOpinionAttitudeType { OPINION_ATTITUDE_LOVE = 12 };

extern const float g_zero;                                  // 0x0122DEDC (0.0f)

struct CThingBuilding;

struct CTCWife {                                            // retail layout: engine/CTCWife.h up to the GiftsReceived map,
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
    bool IsAttractedToHero() const;
};

bool CTCWife::IsAttractedToHero() const
{
    return IsCourtingHero() && (LoveAttitudeValue > g_zero || IsInLoveWithHero());
}