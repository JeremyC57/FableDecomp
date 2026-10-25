// CAudioSource::ApplyVolume(void* dest) -- scale the mixed block at dest by the voice volume through the master's scaler: ramp from the current to the target volume (1/128 of the difference per step) when a ramp is pending, else the constant volume
// Leaf: ?ApplyVolume@CAudioSource@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C54EA0, length 129 bytes (+ int3 padding); `ret 4`.
// Callees: the master scaler's Ramp 0x00C6AC90 / Scale 0x00C6AC60 (thiscall on g_pAudioMaster->m_pScaler).
// Caller: the voice mixer 0x00C54F30 (slot 12, 0x00C54FD2). The catalog label "CAudioSource_OnKill" is propagated
// (CTCCarryable::OnKill); the class keeps the catalog's descriptive name.
// Shape: the scaler is re-read for the ramp call after the flag store (the byte store may alias the global chain); the
// signed `/ 128` is `cdq ; and edx,7Fh ; add ; sar 7`; the 16-bit volume is loaded once into cx for both the difference
// and the push.
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
    void ApplyVolume(void* dest);
};

void CAudioSource::ApplyVolume(void* dest)
{
    if (!g_pAudioMaster->m_pScaler)
        return;
    if (m_ramping) {
        m_ramping = false;
        g_pAudioMaster->m_pScaler->Ramp(dest, m_volume, (m_targetVolume - m_volume) / 128);
        m_volume = m_targetVolume;
    } else {
        g_pAudioMaster->m_pScaler->Scale(dest, m_volume);
    }
}