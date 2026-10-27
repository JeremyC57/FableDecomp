// CVtable_012bf014::Seek(unsigned long position, bool exact) -- the test tone is endless and stateless in position: any seek succeeds and returns the requested position
// Leaf: ?Seek@CVtable_012bf014@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C73EC0, 7 bytes, thiscall, ret 8. No callees.
// Identity: slot 2 of vtable 0x012BF014 (xref 0x012BF01C); the interface slot is COggVorbisStream::Seek 0x00C88640
//   (position, exact flag; returns the new position or -1). Descriptive names (libpk: no PDB/RTTI).
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

long CVtable_012bf014::Seek(unsigned long position, bool exact)
{
    return position;
}