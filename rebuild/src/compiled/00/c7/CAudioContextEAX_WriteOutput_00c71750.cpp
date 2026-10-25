// CAudioContextEAX::WriteOutput(short* output) -- copy the 128-sample 32-bit mix block of the base into the 16-bit output, saturating each sample to the short range
// Leaf: ?WriteOutput@CAudioContextEAX
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C71750, length 203 bytes (+ int3 padding), `ret 4`.
// Module: libpk's OpenAL EAX 3.0 environment (vtable 0x012BEDF4; landed as CAudioContextEAX by its destructor
//   0x00C71CF0, which mutes the listener through EAXSet with DSPROPSETID_EAX30_ListenerProperties 0x012BEE90). This
//   is its slot 7 (0x012BEE10), the base (vtable 0x012B95C4, ctor 0x00C6AD70) leaving the slot an empty `ret 4`
//   0x00C540F0; the base ctor aims +0x21c at the 16-byte-aligned mix block inside the object (this+0x1b & ~15).
// Layout: local model (no PDB/RTTI; names descriptive): base vptr +0, mix block pointer +0x21c.
// Shape: the source loop is hand-unrolled by four with an unsigned counter (`cmp eax,200h ; jb`: the counter becomes
//   the byte offset, the output a pointer stepped by 8); every element re-reads the +0x21c member because the short
//   stores may alias it; the saturation is an inline copy of the out-of-line LTCG helper 0x00C716E0.
class CAudioContextBase {                              // vtable 0x012B95C4
public:
    virtual void v00();
    unsigned char _pad004[0x218];
    long*         m_pMix;                              // +0x21c 16-byte-aligned 128-sample mix block
};

class CAudioContextEAX : public CAudioContextBase {    // vtable 0x012BEDF4
public:
    virtual void v00();
    void WriteOutput(short* output);
};

inline short ClampSample(long value)
{
    if (value > 0x7fff)
        return 0x7fff;
    if (value < -0x8000)
        return -0x8000;
    return (short)value;
}

void CAudioContextEAX::WriteOutput(short* output)
{
    for (unsigned long i = 0; i < 128; i += 4) {
        output[i + 0] = ClampSample(m_pMix[i + 0]);
        output[i + 1] = ClampSample(m_pMix[i + 1]);
        output[i + 2] = ClampSample(m_pMix[i + 2]);
        output[i + 3] = ClampSample(m_pMix[i + 3]);
    }
}