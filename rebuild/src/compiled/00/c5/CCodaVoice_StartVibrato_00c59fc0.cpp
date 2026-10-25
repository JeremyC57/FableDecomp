// CCodaVoice::StartVibrato(const SVibratoParams* vib) -- (re)start the voice's vibrato: compute its rate and depth through SetVibrato, then arm the vibrato delay counter (+0x48) with the description's delay converted from seconds to mix blocks
// Leaf: ?StartVibrato@CCodaVoice@@QAEXPBUSVibratoParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C59FC0, 37 bytes (+ int3 padding), thiscall `ret 4`.
// Retail bytes:  push esi ; push edi ; mov edi,[esp+0Ch] ; push edi ; mov esi,ecx ; call SetVibrato ; test edi,edi ; je done ;
//   mov eax,[edi] ; push eax ; call SecondsToMixBlocks ; add esp,4 ; mov [esi+48h],eax ; done: pop edi ; pop esi ; ret 4
// Identity (sweep1, 2026-10-25; FINDINGS chunk 441 "0xC59FC0 "CMaterialState::SetAnimatedStage" = CCodaVoice::StartVibrato
//   (models callee 0xC59B00 as long(long), it takes float seconds)"):
//   - `this` is the GameCODA voice base (vtable 0x012B5C10, ctor 0x00C59BA0): 0x00C59C60 is agent441's CCodaVoice::SetVibrato
//     (rate +0x40/+0x44, period +0x4c from the SVibratoParams rate/depth), and +0x48 is the vibrato counter the per-block
//     update 0x00C59890 counts down;
//   - 0x00C59B00 is SecondsToMixBlocks (retail inline-asm `fld`/`fmul`/`fistp`, parked by agent441): its argument is the
//     float delay at SVibratoParams +0, pushed through eax because it is only copied (`mov eax,[edi] ; push eax`);
//   - the same "convert the delay at +0" pattern starts the pitch envelope and the second LFO inside SetParameters
//     0x00C5BD00 (agent441's inline StartPitchEnvelope / StartModulation).
//   The CMaterialState name is a propagated manifest label; descriptive names (GameCODA: no PDB/RTTI).
long __cdecl SecondsToMixBlocks(float seconds);         // 0x00C59B00 (seconds -> mix blocks of the output rate)

struct SVibratoParams {                                 // vibrato description
    float delay;                                        // +0x00 seconds before the vibrato starts
    float rate;                                         // +0x04 Hz
    float depth;                                        // +0x08 octaves
};

class CCodaVoice {                                      // vtable 0x012B5C10
public:
    virtual void v00();
    unsigned char _pad04[0x48 - 4];
    long  m_vibratoCounter;                             // +0x48 mix blocks until the next vibrato step
    long  m_vibratoPeriod;                              // +0x4c
    void SetVibrato(const SVibratoParams* vib);         // 0x00C59C60
    void StartVibrato(const SVibratoParams* vib);
};

void CCodaVoice::StartVibrato(const SVibratoParams* vib)
{
    SetVibrato(vib);
    if (vib)
        m_vibratoCounter = SecondsToMixBlocks(vib->delay);
}
