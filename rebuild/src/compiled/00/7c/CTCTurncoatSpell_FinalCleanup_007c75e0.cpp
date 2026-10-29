// CTCTurncoatSpell::FinalCleanup() -- fades out the out-of-range turncoating effect thing if it is still alive, then stops the remaining turncoating effects
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C75E0, length 60 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times: null, dead bit +0x91 & 1, the target), the
// effect thing's fade-out @ 0x0072AC40 (called with the thing in ecx and no argument; the manifest's
// "CTCPhysicalShieldSpell::FadeOutEffect(CThing*)" class/arity is propagated), CTCTurncoatSpell::
// StopTurncoatingEffects() @ 0x007C6DA0 (tail jump).
// Shape: the landed IS_THING_ALIVE idiom (GetPItem re-fetched per use) on &member kept in esi; the trailing
// non-virtual call becomes `mov ecx,edi ; pop ; pop ; jmp`.
// Layout: engine/CTCTurncoatSpell.h is the PDB layout; retail members after Timer sit 4 bytes lower
// (StopTurncoatingEffects 0x007C6DA0 reads LoopingSound at +0x50 = PDB +0x54 and the three effect pointers at
// +0x74 / +0x7C / +0x84 = PDB +0x78 / +0x80 / +0x88), so TurncoatingEffectOnMeOutOfRange is at +0x84 and a local
// model is used.
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags;                  // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    void FadeOutEffect();                   // 0x0072AC40
};

class CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;                        // +0x00
    void* PData;                            // +0x04
    CBaseObject* GetPItem() const;               // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CTCTurncoatSpell { public:
    void*                   __vftable;      // +0x00
    unsigned char           _pad_0x04[0x80];
    CBaseIntelligentPointer TurncoatingEffectOnMeOutOfRange;   // +0x84
    void StopTurncoatingEffects();          // 0x007C6DA0
    void FinalCleanup();
};

void CTCTurncoatSpell::FinalCleanup()
{
    if (IS_THING_ALIVE(TurncoatingEffectOnMeOutOfRange))
        ((CThing*)TurncoatingEffectOnMeOutOfRange.GetPItem())->FadeOutEffect();
    StopTurncoatingEffects();
}