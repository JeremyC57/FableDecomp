// CAudioSource::Reset() -- rewind the software mixer voice: rewind its decoded stream (+0x220, virtual slot 1), reset the two ramped 3-vector records (+0xdc, +0x100) and clear the words at +0x22c and +0x238
// Leaf: ?Reset@CAudioSource@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C55030, length 64 bytes, thiscall.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 440 "0xC55030 CBodyOwner = CAudioSource::Reset"): agent440's landed
//   CAudioSource::Start 0x00C551D0 (slot 10 of the voice vtable at 0x012B8B10) inlines exactly this body after Configure, and its
//   CAudioSource layout (ApplyVolume 0x00C54EA0, Open 0x00C55680) puts the decoded stream at +0x220 and the two SRampedVector
//   records (Reset 0x00C6B720, bound ?Reset@SRampedVector@@QAEXXZ) at +0xdc / +0x100. Descriptive names (no PDB covers libpk).
// Label correction: the landed row was CBodyOwner_00c55030::Reset over "rigid-body records" (the chunk label
//   "CGameScriptInterface::Validate" is propagated); the same callees, typed as the audio source's.
// Shape: one zero register for the two clears, `this` in esi.
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

struct CAudioSource {                              // the libpk software mixer voice
    void*           __vftable;                     // +0x00
    unsigned char   _pad4[0xd8];
    SRampedVector   m_rampA;                       // +0xdc
    SRampedVector   m_rampB;                       // +0x100
    unsigned char   _pad124[0xfc];
    CDecodedStream* m_pStream;                     // +0x220
    unsigned char   _pad224[8];
    long            m_22c;                         // +0x22c
    unsigned char   _pad230[8];
    long            m_238;                         // +0x238
    void Reset();
};

void CAudioSource::Reset()
{
    m_pStream->Rewind();
    m_rampA.Reset();
    m_rampB.Reset();
    m_22c = 0;
    m_238 = 0;
}
