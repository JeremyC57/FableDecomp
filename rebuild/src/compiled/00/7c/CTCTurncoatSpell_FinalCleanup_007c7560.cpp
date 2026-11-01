// CTCTurncoatSpell::FinalCleanup() -- fade out the out-of-range turncoating effect thing if it is still alive, then stop the remaining turncoating effects
// Leaf: ?FinalCleanup@CTCTurncoatSpell@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C7560, 60 bytes. No caller and no vtable reference: the non-virtual private member
//   whose two callers inline it.
// Identity (proven): CTCTurncoatSpell's TU holds three byte-identical bodies in a row: 0x007C7560 (unreferenced),
//   0x007C75A0 (vtable 0x0126E5C4 slot 14 = CTCBase::OnKill) and 0x007C75E0 (slot 2 = CTCBase::OnRemove; landed under the
//   name FinalCleanup). Ego_r.pdb lists the same three in the same order: ?FinalCleanup@CTCTurncoatSpell@@AAEXXZ 0x007C6420,
//   ?OnKill@CTCTurncoatSpell@@UAEXXZ 0x007C6470, ?OnRemove@CTCTurncoatSpell@@UAEXXZ 0x007C64C0 (FableWin.pdb has FinalCleanup
//   too): OnKill and OnRemove each call FinalCleanup, which VC7.1 inlined into both and still emitted out of line here.
//   The catalog label of all three rows is "FinalCleanup"; only this one is.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times: null, dead bit +0x91 & 1, the target),
//   CTCSpecialAbilities::FadeOrKillEffect(CThing*) @ 0x0072AC40 (static __fastcall: the thing in ecx, no stack argument;
//   Ego_r.pdb orders CreateParticleEffect, FadeOrKillEffect(CIntelligentPointer<CThing>&), FadeOrKillEffect(CThing*) as retail
//   0x0072AB70, 0x0072ABD0 (landed), 0x0072AC40 do), CTCTurncoatSpell::StopTurncoatingEffects() @ 0x007C6DA0 (private, tail jump).
// Shape: the landed IS_THING_ALIVE idiom (GetPItem re-fetched per use) on &member kept in esi; the trailing non-virtual call
//   becomes `mov ecx,edi ; pop ; pop ; jmp`.
// Layout: engine/CTCTurncoatSpell.h is the PDB layout; retail members after Timer sit 4 bytes lower (StopTurncoatingEffects
//   0x007C6DA0 reads LoopingSound at +0x50 = PDB +0x54 and the three effect pointers at +0x74 / +0x7C / +0x84 = PDB +0x78 /
//   +0x80 / +0x88), so TurncoatingEffectOnMeOutOfRange is at +0x84 and a local model is used.
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags;                  // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
};

class CBaseObject;
class CBaseIntelligentPointer { public:
    void* __vftable;                        // +0x00
    void* PData;                            // +0x04
    CBaseObject* GetPItem() const;          // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CTCSpecialAbilities { public:
    static void __fastcall FadeOrKillEffect(CThing* effect);   // 0x0072AC40
};

class CTCTurncoatSpell { public:
    virtual void OnRemove();                // CTCBase slot 2 (0x007C75E0)
    virtual void OnKill();                  // CTCBase slot 14 (0x007C75A0)
    unsigned char           _pad_0x04[0x80];
    CBaseIntelligentPointer TurncoatingEffectOnMeOutOfRange;   // +0x84
private:
    void StopTurncoatingEffects();          // 0x007C6DA0
    void FinalCleanup();
};

void CTCTurncoatSpell::FinalCleanup()
{
    if (IS_THING_ALIVE(TurncoatingEffectOnMeOutOfRange))
        CTCSpecialAbilities::FadeOrKillEffect((CThing*)TurncoatingEffectOnMeOutOfRange.GetPItem());
    StopTurncoatingEffects();
}