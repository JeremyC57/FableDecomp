// COggVorbisStream::`scalar deleting destructor'(unsigned int flags) -- compiler-generated: the destructor (0x00C885C0), then the CRT's operator delete (0x00BFE9BC) when bit 0 of the flags is set; returns `this`
// Leaf: ??_GCOggVorbisStream@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C88870, 28 bytes (+ int3 padding), thiscall `ret 4`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 445 "0xC88870 = COggVorbisStream scalar deleting dtor"): slot 0 of the
//   COggVorbisStream vtable 0x012BF1C4 (its only reference), stored by agent445's landed constructor 0x00C88780; it calls
//   COggVorbisStream::~COggVorbisStream 0x00C885C0 and frees with the CRT's 0x00BFE9BC (libpk's streams have no class
//   allocator). The landed CAInputProcess name is a propagated manifest label; descriptive names (libpk: no PDB/RTTI).
// Shape: the compiler-generated `??_G`; the vtable (and so `??_G`) is emitted by the inline constructor and the factory
//   below, the destructor stays an out-of-line call.
struct OggVorbis_File {                                 // libvorbis 1.0.1 OggVorbis_File without ov_callbacks, 0x2C0 bytes
    __int64 _body[0x2c0 / 8];                           //   (full layout in the COggVorbisStream constructor row 0x00C88780)
};

int __fastcall ov_clear(OggVorbis_File* vf);    // 0x00C8A7E0

struct SStreamSegment;

class PKCBufferedReader {                               // vtable 0x012B8458, 0x70 bytes (.\PKCBufferedReader.cpp)
public:
    PKCBufferedReader(void* source, const SStreamSegment& segment);   // 0x00C4B300
    ~PKCBufferedReader();                                // 0x00C4B270
    virtual void v00();
private:
    unsigned char _body04[0x6c];
};

class IAudioStream {                                    // vtable 0x012BEFFC (inline destructor; out-of-line copy 0x00C88890)
public:
    virtual ~IAudioStream() { }
    virtual long GetLength() = 0;
    virtual long Seek(unsigned long position, bool exact) = 0;
    virtual long Read(void* buffer, unsigned long size) = 0;
    virtual long GetRate() = 0;
    virtual long GetState() = 0;
};

class COggVorbisStream : public IAudioStream {          // vtable 0x012BF1C4
public:
    COggVorbisStream(void* source, const SStreamSegment& segment)  // out of line at 0x00C88780 (opens the file)
        : m_reader(source, segment) { }
    virtual ~COggVorbisStream();                        // 0x00C885C0
    virtual long GetLength();                           // 0x00C88620
    virtual long Seek(unsigned long position, bool exact);   // 0x00C88640
    virtual long Read(void* buffer, unsigned long size);     // 0x00C886B0
    virtual long GetRate();                             // 0x00C88740
    virtual long GetState();                            // 0x00C88770
private:
    PKCBufferedReader m_reader;                         // +0x04
    long           m_valid;                             // +0x74
    long           m_opened;                            // +0x78
    OggVorbis_File m_vf;                                // +0x80
};

COggVorbisStream* Create_COggVorbisStream(void* source, const SStreamSegment& segment)
{
    return new COggVorbisStream(source, segment);
}
