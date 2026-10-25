// CombatSequence_AlignDuration(CCombatSequenceBase* sequence, unsigned long time, unsigned long extra) -- the wait from (phase of `time` in the sequence period + extra) up to the next period boundary
// Leaf: CombatSequence_AlignDuration
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1E990, 44 bytes (`ret 4`). __fastcall: sequence in ecx, time in edx, extra on
//   the stack (the landed caller 0x00C1E9F0 GetAlignedDuration declares it so). The manifest name is kept: it says what
//   the body does.
// Callees: the phase helper @ 0x00C1E970 (landed as CCombatSequenceBase_IsBoastSequence: `__fastcall(sequence,
//   value)` returning value % period; edx is passed straight through), the sequence's virtual slot 2 (period).
// Shape: t = phase + extra lands in esi (named local), the period in ecx; `lea eax,[ecx+esi-1] ; div ; imul ; sub`
//   is (t + period - 1) / period * period - t on unsigned values.
struct CCombatSequenceBase {
    virtual unsigned long v0();
    virtual unsigned long v1();
    virtual unsigned long GetPeriod();          // slot 2 (+8)
};

unsigned long __fastcall CCombatSequenceBase_IsBoastSequence(CCombatSequenceBase* sequence, unsigned long value);   // 0x00C1E970

unsigned long __fastcall CombatSequence_AlignDuration(CCombatSequenceBase* sequence, unsigned long time, unsigned long extra)
{
    unsigned long t = CCombatSequenceBase_IsBoastSequence(sequence, time) + extra;
    unsigned long period = sequence->GetPeriod();
    return (t + period - 1) / period * period - t;
}