// CVoiceEffect::Configure(const SVoiceEffectParameters* parameters, long mode) -- set up a voice's +0x2f8 effect from the voice parameter block: configure the +0x260 part (0x00C783F0) and, through its vtable slot 1, the +0x28c stage, then derive the delay length m_delaySamples = m_blockLength * trunc(parameters->DelayScale)
// Grade: DIFFER(108v92) -- retail_asm candidate: retail truncates with a bare `fistp dword` (libpk inline-asm helper, 0x00C6AF2B); ours rounds through C++ sign arms + __ftol2. Both calls (incl. the vtable slot-1 call through the stage pointer) and the imul tail line up | 1 grade | next: land as retail_asm (minimal inline-asm `fld ; fistp` helper + `// Retail-asm:` line)
// Leaf: ?Configure@CVoiceEffect
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6AEF0, 92 bytes (+ int3 padding), `ret 8`.
// Module: libpk's software mixer (no PDB/RTTI; class and member names descriptive). Callers 0x00C544FE / 0x00C546BE
//   configure the effect at voice +0x2f8 from the voice's +0x290 parameter block with mode 0, right after the two float
//   setters 0x00C53740 / 0x00C537A0 on that block.
// Callees: 0x00C783F0 (thiscall on +0x260; the manifest's "CAudioChannel_ProcessEffects" is propagated), the +0x28c stage's
//   vtable slot 1 (thiscall, (owner, mode)).
// Retail-asm: 0x00C6AF1A `fld [edi+10h] ; fsub [0.5f] ; fstp [esp+0Ch] ; fld [esp+0Ch] ; fistp dword [esp+10h] ; test ; jge ;
//   inc` -- libpk's inline-assembly truncation helper (value homed in the dead `parameters` slot, result in the dead `mode` slot).
// Layout: local model. Retail reaches the +0x28c stage through its vtable (`mov eax,[esi+28Ch] ; call [eax+4]`), so the
//   model calls it through a pointer (a member called by name is devirtualised).
// libpk's inline-assembly float-to-long helper. Retail homes the value in a dead stack slot and rounds it with a bare
// `fld [slot] ; fistp dword [tmp]` (round to nearest, the FPU default). VC7.1 C++ cannot emit that (`(long)x` calls
// __ftol2 and /QIfist emits `fistp qword`), so the body below is a C++ stand-in; at landing it becomes
// the minimal inline-assembly `fld value ; fistp result` body (grade retail_asm, like the landed ROUND of 0x00BE6790).
__forceinline long FloatToLong(float value)
{
    return (long)(value < 0.0f ? value - 0.5f : value + 0.5f);     // stand-in: round to nearest (fistp: ties to even)
}
// truncation toward zero on top of the rounding helper: round(x - 0.5) is floor(x), negatives step back up by one
// (retail: `fsub [0.5f] ; fstp [slot] ; fld [slot] ; fistp dword [tmp] ; test ; jge ; inc`)
__forceinline long FloatToLongTruncate(float value)
{
    long result = FloatToLong(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}
struct CVoiceEffect;
struct SVoiceEffectParameters {
    unsigned char _pad00[0x10];
    float         DelayScale;                      // +0x10 delay length in blocks
};
struct CEffectPart260 {
    void Configure(CVoiceEffect* owner, const SVoiceEffectParameters* parameters);   // 0x00C783F0
};
struct CEffectStage28c {
    virtual void v00();
    virtual void Configure(CVoiceEffect* owner, long mode);                       // slot 1
};
struct CVoiceEffect {
    unsigned char   _pad000[0x23c];
    long            m_blockLength;                 // +0x23c samples per block
    unsigned char   _pad240[0x260 - 0x240];
    CEffectPart260  m_part260;                     // +0x260
    unsigned char   _pad261[0x28c - 0x261];
    CEffectStage28c m_stage28c;                    // +0x28c (polymorphic)
    unsigned char   _pad290[0x524 - 0x290];
    long            m_delaySamples;                // +0x524
    void Configure(const SVoiceEffectParameters* parameters, long mode);
};
void CVoiceEffect::Configure(const SVoiceEffectParameters* parameters, long mode)
{
    m_part260.Configure(this, parameters);
    CEffectStage28c* stage = &m_stage28c;
    stage->Configure(this, mode);
    m_delaySamples = m_blockLength * FloatToLongTruncate(parameters->DelayScale);
}
