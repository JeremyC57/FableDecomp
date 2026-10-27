// CLimitedMixBuffer::Reset() -- slot 1 of the limited mix buffer: clear each of the six output limiters at +0xC34 (CLimiter::Clear)
// Leaf: ?Reset@CLimitedMixBuffer@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3F000, 35 bytes, thiscall, plain `ret`.
// Identity: slot 1 of the CLimitedMixBuffer vtables 0x012B5BE0 / 0x012B5D40 / 0x012B5DE0 (CMixBuffer's slot 1 is Reset
//   0x00C59240 in the landed CMixBuffer model); the landed CLimiter::Clear 0x00C592D0 names this as its caller over the six
//   0x17C-byte limiters the constructor 0x00C3EFA0 builds. Descriptive (GameCODA: no PDB/RTTI).
// Shape: `for (i = 0; i < 6; ++i)` becomes a down-counter (edi = 6) and a pointer stride of 0x17C.
class CPKObject {                                       // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
};

class CMixBuffer : public CPKObject {                   // vtable 0x012B8F38, 0xC34 bytes
public:
    virtual ~CMixBuffer();                              // 0x00C57EF0
    virtual void Reset();                               // slot 1 (0x00C59240)
    long*         m_pChannel[6];                        // +0x04
    long          m_active;                             // +0x1c
    unsigned long m_numChannels;                        // +0x20
    unsigned char m_storage[0xc10];                     // +0x24
};

class CLimiter {                                        // vtable 0x012B8F64, 0x17C bytes
public:
    virtual ~CLimiter();                                // 0x00C592C0
    void Clear();                                       // 0x00C592D0
    long          m_delay[88];                          // +0x004
    unsigned long m_writeIndex;                         // +0x164
    float         m_peak;                               // +0x168
    float         m_envelope;                           // +0x16c
    long          m_holdCount;                          // +0x170
    long          m_state;                              // +0x174
    long          m_releaseCount;                       // +0x178
};

class CLimitedMixBuffer : public CMixBuffer {           // vtable 0x012B5BE0
public:
    virtual void Reset();
    CLimiter m_limiters[6];                             // +0xc34
};

void CLimitedMixBuffer::Reset()
{
    for (int i = 0; i < 6; ++i)
        m_limiters[i].Clear();
}