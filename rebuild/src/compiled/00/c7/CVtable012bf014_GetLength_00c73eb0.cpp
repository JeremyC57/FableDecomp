// CVtable_012bf014::GetLength() -- the test-tone stream's length in frames: one hour at its sample rate (rate * 3600)
// Leaf: ?GetLength@CVtable_012bf014@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C73EB0, 10 bytes, thiscall, plain ret. No callees.
// Identity: slot 1 of vtable 0x012BF014 (xref 0x012BF018), the table the landed CVtable_012bf014 constructor 0x00C73EF0
//   stores; its six slots line up with the IAudioStream interface the landed COggVorbisStream (0x00C88780) implements
//   (deleting dtor, GetLength, Seek, Read, GetRate 0x00C73ED0 = +0xc, GetState 0x00C73EE0 = 2). libpk has no PDB/RTTI:
//   the class keeps the landed vtable name, the method name is the interface slot's (descriptive).
// Layout: step +4 (0.0626894 = 2*pi*440/44100), phase +8, sample rate +0xc (44100).
struct SAudioBuffer {                                   // the interleaved 16-bit block a stream's Read fills
    short*        m_data;                               // +0x00
    unsigned long m_channels;                           // +0x04
};

class IAudioStream {                                    // vtable 0x012BEFFC (landed COggVorbisStream model)
public:
    virtual ~IAudioStream() {}
    virtual long GetLength() = 0;                                      // slot 1
    virtual long Seek(unsigned long position, bool exact) = 0;         // slot 2
    virtual long Read(SAudioBuffer* buffer, unsigned long frames) = 0; // slot 3
    virtual long GetRate() = 0;                                        // slot 4
    virtual long GetState() = 0;                                       // slot 5
};

class CVtable_012bf014 : public IAudioStream {          // vtable 0x012BF014: libpk's 440 Hz test-tone stream
public:
    CVtable_012bf014(long unused);                      // 0x00C73EF0
    virtual long GetLength();                           // 0x00C73EB0
    virtual long Seek(unsigned long position, bool exact);   // 0x00C73EC0
    virtual long Read(SAudioBuffer* buffer, unsigned long frames);   // 0x00C73F10
    virtual long GetRate();                             // 0x00C73ED0
    virtual long GetState();                            // 0x00C73EE0
    float m_step;                                       // +0x04 phase step per frame (2*pi*440/44100)
    float m_phase;                                      // +0x08
    long  m_sampleRate;                                 // +0x0c 44100
};

long CVtable_012bf014::GetLength()
{
    return m_sampleRate * 3600;
}