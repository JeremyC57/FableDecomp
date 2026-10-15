// CSequenceClient00c1e9f0::GetAlignedDuration() -- 0 when the owner (+4) has no combat sequence (+4 of the owner); else CombatSequence_AlignDuration(sequence, this->vfunc6(), m_extra (+8))
// Leaf: GetAlignedDuration
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1E9F0, length 43 bytes (+ int3 padding).
// Callees: this's virtual slot 6 (`call [edx+18h]`, no arguments), CombatSequence_AlignDuration @ 0x00C1E990
// (`__fastcall(sequence, time, extra)`: ecx, edx and one stack word, `ret 4`; it rounds `IsBoastSequence-based start +
// extra` up to the sequence's period).
// Label correction: the manifest's "CThingFilter_In3DRange::operator()" is propagated -- the body takes no argument and
// lives in the 0x00C1xxxx module next to the landed CCombatSequenceBase/QueryDistance rows (0x00C1E970, 0x00C1E280,
// which calls this row as its "First" query). Class modelled from the bytes and named by address.
// Shape: right-to-left argument evaluation -- `push [esi+8]` precedes the virtual call, the sequence pointer is
// re-read through the owner after it.
struct CCombatSequenceBase;

long __fastcall CombatSequence_AlignDuration(CCombatSequenceBase* sequence, long time, long extra);   // 0x00C1E990

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
    long GetAlignedDuration();
};

long CSequenceClient00c1e9f0::GetAlignedDuration()
{
    if (m_pOwner->m_pSequence == 0)
        return 0;
    return CombatSequence_AlignDuration(m_pOwner->m_pSequence, GetTime(), m_extra);
}