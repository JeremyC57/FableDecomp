// CAudioSource::Start(CSoundDesc* desc, CPlayParams* params) -- add a reference to the description, configure the voice for it, force unit gains unless the description keeps its own, add the loop capability bits and reset the voice (inline Reset), returning the capability word
// Leaf: ?Start@CAudioSource@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C551D0, length 153 bytes (+ int3 padding); `ret 8`; slot 10 of the voice
// vtable (referenced at 0x012B8B38).
// Callees: the description's slot 1 (AddRef), Configure 0x00C5BD00 (thiscall on the voice), then the landed Reset
// 0x00C55030 inlined: the stream's slot 1, SRampedVector::Reset 0x00C6B720 twice, the +0x22c / +0x238 clears.
// The catalog label "CTCTavernGameCoinGolf_UpdateRotation" is propagated.
// Shape: the capability word lives in edi (`or edi,1BF8000h`); `params->m_loopStart != 0` / `m_loopCount != 0` share
// the zero register ebx (the dead description pointer) with the final clears.
// The libpk software mixer voice (vtable 0x012B8B10 region; no PDB/RTTI, names descriptive): the decoded stream +0x220
// (polymorphic, slot 1 = Rewind), the left / right gains +0x218 / +0x21c, two ramped 3-vector records +0xdc / +0x100 (the
// landed SBodyRecord_00c55030 / Reset 0x00C6B720), the packed 16-bit volume +0x23c, its ramp target +0x23e and the
// ramp flag +0x240. The landed 0x00C55030 (CBodyOwner_00c55030::Reset) is this class's Reset.
struct SRampedVector {                             // 0x24 bytes: current, rest, rate
    float m_current[3];
    float m_rest[3];
    float m_rate[3];
    void Reset();                                  // 0x00C6B720
};

struct CDecodedStream {
    virtual void v00();
    virtual void Rewind();                         // slot 1 (+0x04)
};

struct CVolumeScaler {                             // the master object's +0x17c member
    void Ramp(void* dest, unsigned short volume, long step);   // 0x00C6AC90
    void Scale(void* dest, unsigned short volume);             // 0x00C6AC60 (landed CSampleScaler_00c6ac60::Scale)
};

struct CAudioMaster {                              // the global master object 0x0143BC2C (0x1d8 bytes)
    unsigned char  _pad[0x17c];
    CVolumeScaler* m_pScaler;                      // +0x17c
};
extern CAudioMaster* g_pAudioMaster;               // 0x0143BC2C

struct CSoundDesc {                                // the sound description handed to Start
    virtual void v00();
    virtual void AddRef();                         // slot 1 (+0x04)
    unsigned char _pad4[8];
    unsigned long m_flags;                         // +0x0c (0x800000: keeps its own gains)
};

struct CPlayParams {
    unsigned char _pad[0x10];
    long          m_loopStart;                     // +0x10
    long          m_loopCount;                     // +0x14
};

struct CStreamHandle {
    void Prime();                                  // 0x00C6E660 (landed CRenderCall_00c6e660::Call)
};

struct CAudioSource {
    void*           __vftable;                     // +0x00
    unsigned char   _pad4[0xd8];
    SRampedVector   m_rampA;                       // +0xdc
    SRampedVector   m_rampB;                       // +0x100
    unsigned char   _pad124[0xec];
    long            m_210;                         // +0x210
    long            m_214;                         // +0x214
    float           m_gainLeft;                    // +0x218
    float           m_gainRight;                   // +0x21c
    CDecodedStream* m_pStream;                     // +0x220
    unsigned char   _pad224[8];
    long            m_22c;                         // +0x22c
    unsigned char   _pad230[8];
    long            m_238;                         // +0x238
    unsigned short  m_volume;                      // +0x23c
    unsigned short  m_targetVolume;                // +0x23e
    bool            m_ramping;                     // +0x240
    unsigned long Configure(CSoundDesc* desc, CPlayParams* params);   // 0x00C5BD00
    void Reset()                                   // 0x00C55030 (landed out of line)
    {
        m_pStream->Rewind();
        m_rampA.Reset();
        m_rampB.Reset();
        m_22c = 0;
        m_238 = 0;
    }
    unsigned long Start(CSoundDesc* desc, CPlayParams* params);     // vtable slot 10
};

unsigned long CAudioSource::Start(CSoundDesc* desc, CPlayParams* params)
{
    desc->AddRef();
    unsigned long caps = Configure(desc, params) | 0x1BF8000;
    if (!(desc->m_flags & 0x800000)) {
        m_gainLeft = 1.0f;
        m_gainRight = 1.0f;
        caps &= ~0x800000;
    }
    if (params->m_loopStart)
        caps |= 0x80;
    else if (params->m_loopCount)
        caps |= 0x100;
    Reset();
    return caps;
}