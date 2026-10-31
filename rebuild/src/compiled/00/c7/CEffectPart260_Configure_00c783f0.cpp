// CEffectPart260::Configure(CVoiceEffect* owner, SVoiceEffectParameters* parameters) -- apply a voice effect's dirty parameter flags: run each flagged recalculation stage (filters, envelopes, delay, gain), then clear the flag word
// Leaf: ?Configure@CEffectPart260@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C783F0, 405 bytes, thiscall, `ret 8`.
// Module: libpk's software mixer (no PDB/RTTI; class, member and stage names descriptive). Name: bound
//   `?Configure@CEffectPart260@@...` by the landed caller CVoiceEffect::Configure 0x00C6AEF0 (this = voice +0x260, owner =
//   the voice). The binding's `const SVoiceEffectParameters*` is wrong: this body writes the flag word (`or [esi],9106h`,
//   `mov [esi],0`), so the parameter is non-const here (NAME_FIXES.tsv). The manifest's "CAudioChannel_ProcessEffects" is
//   propagated.
// Callees (thiscall stages on this, (owner, parameters), unless noted): 0x00C78150, 0x00C780C0, 0x00C77FC0, 0x00C77800,
//   0x00C78040, 0x00C77C80, 0x00C77B30, 0x00C77AE0, 0x00C77C00, 0x00C783B0 (parameters only); the owner's filters
//   0x00C74BB0 (on owner +0x1c) and 0x00C74AB0 (on owner +0), (value, value, sample rate); MixChannels_00c77d90 (cdecl,
//   bound by its landed callers); the millibel-to-gain helper 0x00C77700 (cdecl, returns float).
// Shape: retail keeps an unreachable arm in the flag-2 step (`or eax,4 ; jne store`, fall-through `call 0x00C780C0 ; jmp`):
//   VC7.1 does not fold `(Flags | 4) == 0`, so the source tests the or'd word and calls the 0x00C780C0 stage on the dead
//   arm (written first: retail's fall-through). The plain `Flags |= 4` is 13 bytes short (392v405).
// Stage names: none of the callees is bound or named by any source; `Stage_<addr>` / `<Helper>_<addr>` are placeholders.
struct SVoiceEffectParameters {
    unsigned long Flags;                           // +0x00 dirty bits
    long          Level;                           // +0x04 millibels
    float         PeakFrequency;                   // +0x08
    float         ShelfFrequency;                  // +0x0c
    unsigned char _pad10[0x1c - 0x10];
    float         PeakGain;                        // +0x1c
    float         ShelfGain;                       // +0x20
    unsigned char _pad24[0x44 - 0x24];
    float         RateScale;                       // +0x44
    unsigned char _pad48[0x4c - 0x48];
    float         Mix[4];                          // +0x4c
};
struct CMixContext {
    unsigned char _pad00[4];
    long          Channels;                        // +0x04
    unsigned long SampleRate;                      // +0x08
};
struct CEffectFilter {
    float Gain;                                    // +0x00
    void SetShelf(float frequency, float gain, unsigned long sampleRate);   // 0x00C74AB0
    void SetPeak(float frequency, float gain, unsigned long sampleRate);    // 0x00C74BB0
};
struct CVoiceEffect {
    CEffectFilter Shelf;                           // +0x00
    unsigned char _pad04[0x1c - 0x04];
    CEffectFilter Peak;                            // +0x1c
    unsigned char _pad20[0x220 - 0x20];
    float         RateStep;                        // +0x220
};
void __cdecl MixChannels_00c77d90(float* source, float* target, long count, long channels);   // 0x00C77D90
float __cdecl MillibelsToGain_00c77700(long millibels);                                      // 0x00C77700
struct CEffectPart260 {
    float         Values[10];                      // +0x00
    CMixContext*  Context;                         // +0x28
    void Stage_00c77800(CVoiceEffect* owner, SVoiceEffectParameters* parameters);
    void Stage_00c77ae0(CVoiceEffect* owner, SVoiceEffectParameters* parameters);
    void Stage_00c77b30(CVoiceEffect* owner, SVoiceEffectParameters* parameters);
    void Stage_00c77c00(CVoiceEffect* owner, SVoiceEffectParameters* parameters);
    void Stage_00c77c80(CVoiceEffect* owner, SVoiceEffectParameters* parameters);
    void Stage_00c77fc0(CVoiceEffect* owner, SVoiceEffectParameters* parameters);
    void Stage_00c78040(CVoiceEffect* owner, SVoiceEffectParameters* parameters);
    void Stage_00c780c0(CVoiceEffect* owner, SVoiceEffectParameters* parameters);
    void Stage_00c78150(CVoiceEffect* owner, SVoiceEffectParameters* parameters);
    void Stage_00c783b0(SVoiceEffectParameters* parameters);
    void Configure(CVoiceEffect* owner, SVoiceEffectParameters* parameters);
};
void CEffectPart260::Configure(CVoiceEffect* owner, SVoiceEffectParameters* parameters)
{
    if (parameters->Flags == 0)
        return;
    if (parameters->Flags & 1) {
        Stage_00c78150(owner, parameters);
        parameters->Flags |= 0x9106;
    }
    if (parameters->Flags & 2) {
        unsigned long flags = parameters->Flags | 4;
        if (flags == 0)
            Stage_00c780c0(owner, parameters);
        else
            parameters->Flags = flags;
    }
    if (parameters->Flags & 4) {
        Stage_00c780c0(owner, parameters);
        parameters->Flags |= 0x9000;
    }
    if (parameters->Flags & 0x48) {
        owner->Peak.Gain = 1.0f;
        owner->Peak.SetPeak(parameters->PeakFrequency, parameters->PeakGain, Context->SampleRate);
    }
    if (parameters->Flags & 0x90)
        owner->Shelf.SetShelf(parameters->ShelfFrequency, parameters->ShelfGain, Context->SampleRate);
    if (parameters->Flags & 0x20000) {
        Stage_00c77fc0(owner, parameters);
        parameters->Flags |= 0x100;
    }
    if (parameters->Flags & 0x40000)
        parameters->Flags |= 0x8000;
    if (parameters->Flags & 0x718)
        Stage_00c77800(owner, parameters);
    if (parameters->Flags & 0x80000)
        owner->RateStep = (float)Context->SampleRate * parameters->RateScale;
    if (parameters->Flags & 0x180000)
        Stage_00c78040(owner, parameters);
    if (parameters->Flags & 0x10000)
        Stage_00c77c80(owner, parameters);
    if (parameters->Flags & 0x12000) {
        Stage_00c77b30(owner, parameters);
        Stage_00c77c80(owner, parameters);
    }
    if (parameters->Flags & 0x800)
        MixChannels_00c77d90(parameters->Mix, Values, 4, Context->Channels);
    if (parameters->Flags & 0x1800)
        Stage_00c77ae0(owner, parameters);
    if (parameters->Flags & 0x4000)
        Stage_00c783b0(parameters);
    if (parameters->Flags & 0xc000)
        Stage_00c77c00(owner, parameters);
    if (parameters->Flags & 0x20)
        owner->Shelf.Gain = MillibelsToGain_00c77700(parameters->Level);
    parameters->Flags = 0;
}