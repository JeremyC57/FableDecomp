// CVtable_012bf014::Read(SAudioBuffer* buffer, unsigned long frames) -- synthesise `frames` frames of the 440 Hz test tone: one 16-bit sample sin(phase) * 32767 written to every channel of the frame, the phase advanced by the step per frame; returns the frame count
// Leaf: ?Read@CVtable_012bf014@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C73F10, 107 bytes, thiscall, ret 8. Callee: __ftol2 0x00BFEA70; the
//   float 32767.0f from the pool 0x0129C820; fsin is the /Oi intrinsic of sin.
// Identity: slot 3 of vtable 0x012BF014 (xref 0x012BF020); the interface slot is COggVorbisStream::Read 0x00C886B0,
//   which reads the same block descriptor (data +0, channels +4; bytes = channels * frames * 2). Descriptive names
//   (libpk: no PDB/RTTI).
// Shape: VC7.1 turns the unsigned frame loop into a down-counter in the dead buffer argument slot (`jbe` entry test); the channel loop re-reads
//   the channel count after every 16-bit store.
#include <math.h>
#pragma intrinsic(sin)
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

long CVtable_012bf014::Read(SAudioBuffer* buffer, unsigned long frames)
{
    short* out = buffer->m_data;
    for (unsigned long i = 0; i < frames; ++i) {
        short sample = (short)(sin(m_phase) * 32767.0f);
        for (unsigned long c = 0; c < buffer->m_channels; ++c)
            *out++ = sample;
        m_phase += m_step;
    }
    return frames;
}