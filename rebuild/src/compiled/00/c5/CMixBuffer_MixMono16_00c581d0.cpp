// CMixBuffer::MixMono16(const short* src, const unsigned short* gains) -- add one mono block of 16-bit samples into every channel accumulator, each under its own Q15 gain (virtual slot 3 MixAdd16 per channel)
// Leaf: ?MixMono16@CMixBuffer@@QAEXPBFPBG@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C581D0, 61 bytes (+ int3 padding), thiscall `ret 8`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 441 "0xC581D0 "CalculateTextureChangeFlags" = CMixBuffer::MixMono16 ("7
//   texture ids" = six channel pointers + count)"):
//   - `this` is the GameCODA mix accumulator (vtable 0x012B8F38, ctor 0x00C591F0 = the landed CMixBuffer::CMixBuffer):
//     the "seven inline ids" at +4 are the six planar channel pointers +4..+0x18 and the active flag +0x1c, the count at
//     +0x20 is the channel count;
//   - slot 3 (+0xc) of 0x012B8F38 is 0x00C580C0, the kernel that adds 16-bit samples scaled by a Q15 gain (agent441's
//     MixAdd16); agent441's landed MixMono16Ramped 0x00C58210 is the ramped twin of this loop (slot 9, gains + steps).
//   The CalculateTextureChangeFlags name is a propagated manifest label; descriptive names (GameCODA: no PDB/RTTI).
// Shape: the unsigned count re-read each pass, the channel pointer walking in ebx; an `unsigned short` gain pushes
//   `xor ecx,ecx ; mov cx,[..]`.
class CMixBuffer {                                      // vtable 0x012B8F38
public:
    virtual ~CMixBuffer();                                                            // slot 0
    virtual void Reset();                                                             // slot 1
    virtual void Clear();                                                             // slot 2
    virtual void MixAdd16(const short* src, long* dst, unsigned short gain);         // slot 3 (0x00C580C0)
    long*         m_pChannel[6];                        // +0x04 planar 128-sample accumulators
    long          m_active;                             // +0x1c
    unsigned long m_numChannels;                        // +0x20
    void MixMono16(const short* src, const unsigned short* gains);
};

void CMixBuffer::MixMono16(const short* src, const unsigned short* gains)
{
    for (unsigned long i = 0; i < m_numChannels; ++i)
        MixAdd16(src, m_pChannel[i], gains[i]);
}
