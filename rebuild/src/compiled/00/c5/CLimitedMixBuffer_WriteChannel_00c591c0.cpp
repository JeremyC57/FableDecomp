// CLimitedMixBuffer::WriteChannel(short* out, unsigned long channel) -- vtable slot 10: write one accumulator channel to the interleaved 16-bit output through that channel's CLimiter (instead of CMixBuffer's plain saturating copy)
// Leaf: ?WriteChannel@CLimitedMixBuffer@@UAEXPAFK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C591C0, 43 bytes (+ int3 padding), thiscall `ret 8`; slot 10 of the
//   CLimitedMixBuffer vtable 0x012B5BE0 and of its derived vtables 0x012B5D40 / 0x012B5DE0 (the landed CLimitedMixBuffer
//   ctor 0x00C3EFA0 names this slot as "channel c's output goes through m_limiters[c] (0x00C593C0) instead of
//   CMixBuffer's 0x00C590E0").
// Callee: CLimiter::Process @ 0x00C593C0 (thiscall, `ret 10h`; the limiter's delay-line/envelope loop, not landed,
//   descriptive name) on m_limiters[channel] (0x17C bytes each, +0xC34).
// Name: descriptive (GameCODA, no PDB/RTTI); CMixBuffer's own slot 10 (0x00C590E0) writes m_pChannel[channel] >> m_shift
//   saturated to 16 bits at a stride of m_numChannels samples, so +0x1C is the output shift (the landed models call it
//   m_active). The manifest label `_gapscan::sub_00c591c0` is a placeholder.
// Shape: arguments pushed right to left (+0x1C first), `imul eax,17Ch` for the limiter index.
class CPKObject {                                       // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
};

class CMixBuffer : public CPKObject {                   // vtable 0x012B8F38, 0xC34 bytes
public:
    virtual ~CMixBuffer();                              // 0x00C57EF0
    virtual void Reset();                               // slot 1
    virtual void Clear();                               // slot 2
    virtual void MixAdd16(const short* src, long* dst, unsigned short gain);
    virtual void MixAdd32Gain(const long* src, long* dst, unsigned short gain);
    virtual void MixAdd32(const long* src, long* dst);
    virtual void MixAddFloatGain(const float* src, long* dst, float gain);
    virtual void MixAddFloatGain16(const float* src, long* dst, unsigned short gain);
    virtual void MixAddFloat(const float* src, long* dst);
    virtual void MixAdd16Ramp(const short* src, long* dst, unsigned short gain, short step);
    virtual void WriteChannel(short* out, unsigned long channel);   // slot 10 (0x00C590E0)
    long*         m_pChannel[6];                        // +0x04 planar 128-sample accumulators
    long          m_active;                             // +0x1c output shift
    unsigned long m_numChannels;                        // +0x20
    unsigned char m_storage[0xc10];                     // +0x24 accumulator store, 16-byte aligned by hand
};

class CLimiter {                                        // vtable 0x012B8F64, 0x17C bytes
public:
    virtual ~CLimiter();                                // 0x00C592C0
    long          m_delay[88];                          // +0x004
    unsigned long m_writeIndex;                         // +0x164
    float         m_peak;                               // +0x168
    float         m_envelope;                           // +0x16c
    long          m_holdCount;                          // +0x170
    long          m_state;                              // +0x174
    long          m_releaseCount;                       // +0x178
    void Process(short* out, unsigned long stride, const long* src, long shift);   // 0x00C593C0
};

class CLimitedMixBuffer : public CMixBuffer {           // vtable 0x012B5BE0
public:
    virtual void WriteChannel(short* out, unsigned long channel);
    CLimiter m_limiters[6];                             // +0xc34
};

void CLimitedMixBuffer::WriteChannel(short* out, unsigned long channel)
{
    m_limiters[channel].Process(out, m_numChannels, m_pChannel[channel], m_active);
}