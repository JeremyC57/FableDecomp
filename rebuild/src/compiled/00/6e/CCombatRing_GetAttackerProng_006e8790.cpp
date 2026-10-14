// CCombatRing::GetAttackerProng(const CThing* attacker) -- the prong whose attacker pointer resolves to this thing, else null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E8790, length 53 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on each prong's attacker pointer (+0x14 of the
// 0x1c-byte CDefendingCombatantInfo::CProng; Prongs is the std::vector at +0x10, Ego_r.pdb).
// Shape: a plain pointer loop over [first, last) with the vector's last pointer RE-READ from this after each
// GetPItem call (`mov eax,[edi+14h]`), the attacker argument loaded once (ebx) in the loop preheader, `xor
// eax,eax` for the not-found tail.
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CProng {                                 // CDefendingCombatantInfo::CProng, sizeof 0x1c
    unsigned char _pad[0x14];
    CBaseIntelligentPointer PAttacker;          // +0x14 (CIntelligentPointer<CThing>)
};

struct CProngVector {                           // std::vector<CProng>
    CProng* m_pFirst;                           // +0x00
    CProng* m_pLast;                            // +0x04
    CProng* m_pEnd;                             // +0x08
};

struct CCombatRing {
    float RotationAngle;                        // +0x00
    float DeltaRotation;                        // +0x04
    long Index;                                 // +0x08
    float RingThickness;                        // +0x0c
    CProngVector Prongs;                        // +0x10
    CProng* GetAttackerProng(const CThing* attacker);
};

CProng* CCombatRing::GetAttackerProng(const CThing* attacker)
{
    for (CProng* prong = Prongs.m_pFirst; prong != Prongs.m_pLast; ++prong) {
        if (prong->PAttacker.GetPItem() == attacker)
            return prong;
    }
    return 0;
}