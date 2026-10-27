// CMixBuffer::MixAdd32(const long* src, long* dst) -- vtable slot 5: add a 128-sample block of 32-bit samples into a long accumulator
// Leaf: ?MixAdd32@CMixBuffer@@UAEXPBJPAJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C58000, 59 bytes (+ int3 padding), thiscall `ret 8`; a slot of the CMixBuffer vtable 0x012B8F38 (shared by
//   the derived vtables 0x012B5BE0 / 0x012B5D40 / 0x012B5DE0 where listed by xref).
// Name: the landed CMixBuffer model (0x00C58370 / 0x00C591F0) already names this slot (descriptive; GameCODA has no
//   PDB/RTTI). The manifest label `_gapscan::sub_00c58000` is a placeholder.
// Shape: VC7.1 unrolls the 128-step loop by 4 (`mov esi,20h ; dec esi ; jne`).
class CMixBuffer {
public:
    virtual ~CMixBuffer();                                                            // slot 0 (0x00C59250)
    virtual void Reset();                                                             // slot 1 (0x00C59240)
    virtual void Clear();                                                             // slot 2 (0x00C57F00)
    virtual void MixAdd16(const short* src, long* dst, unsigned short gain);         // slot 3 (0x00C580C0)
    virtual void MixAdd32Gain(const long* src, long* dst, unsigned short gain);      // slot 4 (0x00C58040)
    virtual void MixAdd32(const long* src, long* dst);                                // slot 5 (0x00C58000)
    virtual void MixAddFloatGain(const float* src, long* dst, float gain);           // slot 6 (0x00C57FC0)
    virtual void MixAddFloatGain16(const float* src, long* dst, unsigned short gain); // slot 7 (0x00C57F60)
    virtual void MixAddFloat(const float* src, long* dst);                            // slot 8 (0x00C57F20)
    virtual void MixAdd16Ramp(const short* src, long* dst, unsigned short gain, short step);   // slot 9 (0x00C58140)
    long*         m_pChannel[6];        // +0x04 planar 128-sample accumulators
    long          m_active;             // +0x1c (1 after construction)
    unsigned long m_numChannels;        // +0x20
    unsigned char m_storage[0xc10];     // +0x24 accumulator store, 16-byte aligned by hand
};

void CMixBuffer::MixAdd32(const long* src, long* dst)
{
    for (int i = 0; i < 128; ++i)
        dst[i] += src[i];
}