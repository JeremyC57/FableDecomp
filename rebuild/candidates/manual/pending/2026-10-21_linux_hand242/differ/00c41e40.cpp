// Grade: DIFFER(368v368) -- scheduling only: in each float arm retail emits mov eax,[flags] ; fstp member ; or eax,bit ; mov [flags],eax (ours stores the float before the flag load)
// CCombatant::SetCombatParameters(const SCombatParameters&) -- copy each combat parameter that changed into the combatant and raise the matching dirty bits (0x1000000 for most, 0x400000 / 0x1400000 for the third to sixth)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C41E40, 368 bytes. `ret 4`, no callees.
//
// Shape: every field goes through an inline by-value setter `if (member != value) { member = value; flags |= bit; }`. The integer fields
// compare memory against the loaded parameter; each float field is loaded once, duplicated and compared with
// `fucompp ; test ah,44h ; jnp` (inequality), then the same x87 copy is stored. The 0x1000000 mask is
// kept in edi. No PDB layout exists for CCombatant (local model, as the landed ComputeSpellPower
// 0x00C41BB0 / ComputeLightingAdjustment 0x00C41A20, whose float at +0x1c4 this body also writes).
// Layout: local model (descriptive member names; the parameter block is 0x28 bytes).
struct SCombatParameters {
    long  Value0;                           // +0x00
    long  Value1;                           // +0x04
    long  Value2;                           // +0x08
    float Value3;                           // +0x0c
    long  Value4;                           // +0x10
    float Value5;                           // +0x14
    long  Value6;                           // +0x18
    float Value7;                           // +0x1c
    float Value8;                           // +0x20
    float Value9;                           // +0x24
};
struct CCombatant {
    unsigned char _pad[0x1a4];
    long          m_param0;                 // +0x1a4
    long          m_param1;                 // +0x1a8
    long          m_param2;                 // +0x1ac
    float         m_param3;                 // +0x1b0
    long          m_param4;                 // +0x1b4
    float         m_param5;                 // +0x1b8
    long          m_param6;                 // +0x1bc
    float         m_param7;                 // +0x1c0
    float         m_spellScale;             // +0x1c4 (read by ComputeSpellPower)
    float         m_param9;                 // +0x1c8
    unsigned char _pad1cc[0xc];             // +0x1cc
    unsigned long m_dirtyFlags;             // +0x1d8
    template <class T> void SetParameter(T& member, T value, unsigned long dirtyBit)
    {
        if (member != value) {
            member = value;
            m_dirtyFlags |= dirtyBit;
        }
    }
    void SetCombatParameters(const SCombatParameters& p);
};

void CCombatant::SetCombatParameters(const SCombatParameters& p)
{
    SetParameter(m_param0, p.Value0, 0x1000000);
    SetParameter(m_param1, p.Value1, 0x1000000);
    SetParameter(m_param2, p.Value2, 0x400000);
    SetParameter(m_param3, p.Value3, 0x400000);
    SetParameter(m_param4, p.Value4, 0x1400000);
    SetParameter(m_param5, p.Value5, 0x1400000);
    SetParameter(m_param6, p.Value6, 0x1000000);
    SetParameter(m_param7, p.Value7, 0x1000000);
    SetParameter(m_spellScale, p.Value8, 0x1000000);
    SetParameter(m_param9, p.Value9, 0x1000000);
}
