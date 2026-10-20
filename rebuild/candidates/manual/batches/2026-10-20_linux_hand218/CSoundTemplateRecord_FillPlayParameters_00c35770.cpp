// CSoundTemplateRecord::FillPlayParameters(CSoundPlayParameters& params) const -- convert a packed sound record (byte volume/pitch/pan, flag bits, frequency) into a play-parameter block through its setters; returns S_OK
// Leaf: FillPlayParameters@CSoundTemplateRecord
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C35770, 309 bytes (`ret 4`, HRESULT 0). Audio library code: the manifest label
// "C3DAnimationSequence::InitializeFromChunk" is propagated and wrong (its only caller 0x00C269AF builds the parameter
// block on the stack with 0x00C1BD10 and tests the result with `jl`); class and setter names here are descriptive.
// Callees (all thiscall on the parameter block, 0x00C1BB20..0x00C1BC70): SetVolume(float) 0x00C1BB20 (byte +0 = v*127),
// SetPitch(float) 0x00C1BB40 (word +8 = v*100), SetPan(float) 0x00C1BB60 (byte +1 = v*100), SetLoopStart(long)
// 0x00C1BB80 / SetLoopEnd(long) 0x00C1BBA0 (converted words +0x10/+0x12), SetPriority(long) 0x00C1BBC0 (+0xa),
// SetFrequencyScale(float) 0x00C1BC70 (word +0xe = v*65535), SetDelay(short) 0x00C1BBD0 (+4), SetFadeIn(unsigned
// short) 0x00C1BBE0 (+6), SetGroup(unsigned short) 0x00C1BBF0 (byte +3), SetCategory(long) 0x00C1BC00 (+0xc),
// SetStreamed(bool) 0x00C1BC30 / SetPositional(bool) 0x00C1BC50 / SetLooped(bool) 0x00C1BC10 (bits 1/2/0 of +2).
// Constants: 1/127 0x0129F780, 0.01f 0x01230A5C, 2^32 0x0122DCB4 (unsigned fild fix-up), 1.0f 0x0122DED8.
// Shape: byte fields converted `(float)b * k`; flag bits of +0xf / +0x10 are unsigned char bit fields (`test byte ptr`,
// the last one passed as a bool: `mov cl ; and ecx,0FFFFFF01h`); the frequency divisor is an unsigned long tested
// against 0 and -1 (`1.0f / (float)n`, else 1.0f); word fields pushed as `xor ; mov cx` go to unsigned short
// parameters, the `movzx` one to a long.
struct CSoundPlayParameters {
    void SetVolume(float v);                       // 0x00C1BB20
    void SetPitch(float v);                        // 0x00C1BB40
    void SetPan(float v);                          // 0x00C1BB60
    void SetLoopStart(long v);                     // 0x00C1BB80
    void SetLoopEnd(long v);                       // 0x00C1BBA0
    void SetPriority(long v);                      // 0x00C1BBC0 (stores the low word)
    void SetDelay(short v);                        // 0x00C1BBD0
    void SetFadeIn(unsigned short v);              // 0x00C1BBE0
    void SetGroup(unsigned short v);               // 0x00C1BBF0
    void SetCategory(long v);                      // 0x00C1BC00
    void SetLooped(bool v);                        // 0x00C1BC10
    void SetStreamed(bool v);                      // 0x00C1BC30
    void SetPositional(bool v);                    // 0x00C1BC50
    void SetFrequencyScale(float v);               // 0x00C1BC70
};

struct CSoundTemplateRecord {
    unsigned char  _00[8];
    unsigned short Category;                       // +0x08
    unsigned short Group;                          // +0x0a
    unsigned short FadeIn;                         // +0x0c
    unsigned char  _0e;
    unsigned char  Streamed : 1;                   // +0x0f bit 0
    unsigned char  Positional : 1;                 // +0x0f bit 1
    unsigned char  : 0;
    unsigned char  Looped : 1;                     // +0x10 bit 0
    unsigned char  HasLoopStart : 1;               // +0x10 bit 1
    unsigned char  HasLoopEnd : 1;                 // +0x10 bit 2
    unsigned char  Volume;                         // +0x11
    unsigned char  Pitch;                          // +0x12
    unsigned char  Pan;                            // +0x13
    long           LoopStart;                      // +0x14
    long           LoopEnd;                        // +0x18
    long           Priority;                       // +0x1c
    unsigned long  FrequencyDivisor;               // +0x20
    long FillPlayParameters(CSoundPlayParameters& params) const;
};

long CSoundTemplateRecord::FillPlayParameters(CSoundPlayParameters& params) const
{
    params.SetVolume((float)Volume * (1.0f / 127.0f));
    params.SetPitch((float)Pitch * 0.01f);
    params.SetPan((float)Pan * 0.01f);
    if (HasLoopStart)
        params.SetLoopStart(LoopStart);
    if (HasLoopEnd)
        params.SetLoopEnd(LoopEnd);
    params.SetPriority(Priority);
    float scale;
    if (FrequencyDivisor != 0 && FrequencyDivisor != 0xffffffff)
        scale = 1.0f / (float)FrequencyDivisor;
    else
        scale = 1.0f;
    params.SetFrequencyScale(scale);
    params.SetDelay(0);
    params.SetFadeIn(FadeIn);
    params.SetGroup(Group);
    params.SetCategory(Category);
    if (Streamed)
        params.SetStreamed(true);
    if (Positional)
        params.SetPositional(true);
    params.SetLooped(Looped);
    return 0;
}
