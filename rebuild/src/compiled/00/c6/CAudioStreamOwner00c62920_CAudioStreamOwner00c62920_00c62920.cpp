// CAudioStreamOwner_00c62920::CAudioStreamOwner_00c62920(IStreamOpener_00c62920* opener, const char* name, const SStreamSegment& segment) -- the 0x14-byte owner of a sound's decoded stream: no source, no stream, position -1, not finished; with the test-tone switch set it owns a new tone stream (constructed with 800.0f) instead; otherwise it opens the named source through the opener (slot 0) and, when that succeeds, aligns the segment start up to the source's block size (slot 2) and creates the matching decoder stream for the name (0x00C73AF0)
// Leaf: ??0CAudioStreamOwner_00c62920@@QAE@PAVIStreamOpener_00c62920@@PBDABUSStreamSegment@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C62920, 256 bytes, thiscall `ret 0Ch`, returns `this`; EH frame (state 0 around the tone
//   stream's constructor after the scalar operator new 0x00BFEA1A of 0x10 bytes). The segment copy is a frame local.
// Identity: the only caller 0x00C4812E constructs it in a fresh 0x14-byte block (operator new 0x14). Its accessors are landed as
//   0x00C628A0 / 0x00C628D0 / 0x00C628F0 (the stream's slots 1 / 4 / 5 or -1 without a stream) and 0x00C628C0 (+0x0C, 0 without a
//   stream) under propagated names ("CAIBrain::GetCurrentStateGroupDebugText", "CEditWorld::DrawGetWorldMap",
//   "CHandleOwner_00c628f0::GetIndex"). libpk has no PDB/RTTI: class and member names are descriptive; the catalog's
//   "C3DGroup2_Create" is propagated.
// Callees: operator new @ 0x00BFEA1A, the tone stream CVtable_012bf014 constructor @ 0x00C73EF0 (it ignores its argument; the caller
//   pushes the float 800.0f 0x44480000), the opener's slot 0, the source's slot 2, the decoder-stream factory @ 0x00C73AF0
//   (__fastcall: source in ecx, name in edx, the segment by reference on the stack; it classifies the name's extension).
// Global: the test-tone switch 0x0143C5B8 (bool).
struct SStreamSegment {                     // the landed COggVorbisStream model's segment (start, length, flags)
    unsigned long m_word0;                  // +0x00 start (aligned here)
    unsigned long m_word4;
    unsigned long m_word8;
};

class IAudioStream {                        // vtable 0x012BEFFC (landed COggVorbisStream model)
public:
    virtual ~IAudioStream() {}
};

class CVtable_012bf014 : public IAudioStream {   // the 440 Hz test tone stream (landed 0x00C73EF0 / 0x00C73F10), 0x10 bytes
public:
    CVtable_012bf014(float frequency);      // 0x00C73EF0 (argument unused)
    float m_scale;                          // +4
    long  m_position;                       // +8
    long  m_sampleRate;                     // +0xc
};

class IStreamSource_00c62920 {              // an opened file source
public:
    virtual void v00();
    virtual void v01();
    virtual unsigned long GetBlockSize();   // slot 2 (+0x08)
};

class IStreamOpener_00c62920 {              // opens a named source
public:
    virtual IStreamSource_00c62920* Open(const char* name);   // slot 0
};

IAudioStream* __fastcall CreateDecoderStream_00c73af0(IStreamSource_00c62920* source, const char* name, const SStreamSegment& segment);   // 0x00C73AF0

inline unsigned long AlignUp_00c62920(unsigned long value, unsigned long alignment)
{
    return (value + alignment - 1) / alignment * alignment;
}

extern bool g_useTestToneStream;            // 0x0143C5B8

class CAudioStreamOwner_00c62920 {          // 0x14 bytes
public:
    CAudioStreamOwner_00c62920(IStreamOpener_00c62920* opener, const char* name, const SStreamSegment& segment);
    IStreamSource_00c62920* m_source;       // +0x00
    IAudioStream*           m_stream;       // +0x04
    long                    m_position;     // +0x08 (-1)
    long                    m_0c;           // +0x0c
    bool                    m_finished;     // +0x10
};

CAudioStreamOwner_00c62920::CAudioStreamOwner_00c62920(IStreamOpener_00c62920* opener, const char* name, const SStreamSegment& segment)
    : m_source(0), m_stream(0), m_position(-1), m_finished(false)
{
    if (g_useTestToneStream) {
        m_stream = new CVtable_012bf014(800.0f);
        return;
    }
    m_source = opener->Open(name);
    if (m_source) {
        SStreamSegment aligned = segment;
        aligned.m_word0 = AlignUp_00c62920(aligned.m_word0, m_source->GetBlockSize());
        m_stream = CreateDecoderStream_00c73af0(m_source, name, aligned);
    }
}