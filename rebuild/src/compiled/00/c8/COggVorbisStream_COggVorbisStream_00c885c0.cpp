// COggVorbisStream::~COggVorbisStream() -- EH-framed destructor of libpk's Ogg Vorbis audio stream: its vtable (0x012BF1C4), ov_clear on the OggVorbis_File (+0x80) when the file was opened (+0x78; EH state 1), the PKCBufferedReader member (+4, ~PKCBufferedReader 0x00C4B270; state 0), then the inline IAudioStream destructor (vtable 0x012BEFFC)
// Leaf: ??1COggVorbisStream@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C885C0, 96 bytes, thiscall, plain `ret`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 445 "0xC885C0 ~CAInputProcess is ~COggVorbisStream (+0x78 opened flag,
//   +0x80 is the OggVorbis_File + ov_clear 0xC8A7E0, +4 is the 0x70-byte CStreamReader, +0x74 valid flag)"):
//   - its only caller is the scalar deleting destructor 0x00C88870, slot 0 of 0x012BF1C4, the vtable agent445's landed
//     COggVorbisStream::COggVorbisStream 0x00C88780 stores (slots 1-5: GetLength 0x00C88620 .. GetState 0x00C88770);
//   - the constructor builds the same members: the reader (source, segment) 0x00C4B300 at +4 (its destructor 0x00C4B270
//     is EH state 1 there), sets the opened flag +0x78 and opens +0x80 with ov_open 0x00C8CAE0; the reader is libpk's
//     PKCBufferedReader (its constructor's and seek's messages name `.\PKCBufferedReader.cpp`; agent445's CStreamReader);
//   - 0x00C8A7E0 is the landed libvorbis 1.0.1 ov_clear (block/dsp/stream clear, every link's info and comment freed).
//   The landed CAInputProcess name is a propagated manifest label; descriptive names (libpk: no PDB/RTTI).
// Shape: ov_clear is declared with C++ linkage, as in the constructor's TU: under /EHsc an extern "C" callee is nothrow
//   and the state-1 store before the test disappears.
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
    COggVorbisStream(void* source, const SStreamSegment& segment);   // 0x00C88780
    virtual ~COggVorbisStream();
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

COggVorbisStream::~COggVorbisStream()
{
    if (m_opened)
        ov_clear(&m_vf);
}
