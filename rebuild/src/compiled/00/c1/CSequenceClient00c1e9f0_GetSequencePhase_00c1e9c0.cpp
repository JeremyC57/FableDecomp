// CSequenceClient00c1e9f0::GetSequencePhase() -- 0 when the owner (+4) has no combat sequence, else this->GetTime() modulo the sequence period
// Leaf: ?GetSequencePhase@CSequenceClient00c1e9f0@@QAEJXZ
// Retail: Fable.exe @ 0x00C1E9C0, 38 bytes (tail jump). Descriptive name (no PDB/RTTI covers the 0x00C1xxxx module).
// Label correction: the manifest row "CWorldMap::GetNextRegionOnRouteTo" is a propagation; this is the sibling of the
// landed CSequenceClient00c1e9f0::GetAlignedDuration 0x00C1E9F0 (same owner/sequence test, same virtual slot 6) and is
// called from 0x00C1D7D3, 0x00C1D7FD, 0x00C1D813, 0x00C1E2CC, 0x00C1E3A1, 0x00C1EA8D, 0x00C2631A.
// Callees: this's virtual slot 6 GetTime() (+0x18), the phase helper 0x00C1E970 (landed as
// CCombatSequenceBase_IsBoastSequence: `__fastcall(sequence, value)` = value % sequence->GetPeriod()).
// Shape: the class model of the landed 0x00C1E9F0; right-to-left argument evaluation calls GetTime() before the
// sequence pointer is re-read through the owner, and the register-only `__fastcall` call is a sibling `jmp`.
class CCombatSequenceBase;

unsigned long __fastcall CCombatSequenceBase_IsBoastSequence(CCombatSequenceBase* sequence, unsigned long value);   // 0x00C1E970

struct CSequenceOwner00c1e9f0 {
    unsigned char        _pad00[4];
    CCombatSequenceBase* m_pSequence;               // +0x04
};

struct CSequenceClient00c1e9f0 {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void v03(); virtual void v04(); virtual void v05();
    virtual long GetTime();                         // slot 6 (+0x18)
    CSequenceOwner00c1e9f0* m_pOwner;               // +0x04
    long                    m_extra;                // +0x08
    long GetSequencePhase();
};

long CSequenceClient00c1e9f0::GetSequencePhase()
{
    if (m_pOwner->m_pSequence == 0)
        return 0;
    return CCombatSequenceBase_IsBoastSequence(m_pOwner->m_pSequence, GetTime());
}