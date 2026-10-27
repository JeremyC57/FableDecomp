// COggVorbisStream::Read(SAudioBuffer* buffer, unsigned long frames) -- decode up to `frames` interleaved 16-bit frames into buffer->m_data: -1 when the stream is not valid, 0 for no frames; otherwise ov_read (little-endian, 16-bit, signed) in a loop until the byte budget is used up or the decoder stops; a decoder error is returned as is, else the frame count decoded (bytes / (channels * 2))
// Leaf: ?Read@COggVorbisStream@@UAEJPAUSAudioBuffer@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C886B0, 137 bytes, thiscall `ret 8`. The bitstream out-local lives in the dead `frames`
// argument slot ([esp+18h]), and the unsigned frame count divides with `div`.
// Identity: slot 3 of the COggVorbisStream vtable 0x012BF1C4 (reference at 0x012BF1D0), the Read slot the landed constructor
// 0x00C88780 and Seek 0x00C88640 document; catalogued as gap-scan sub_00c886b0. The {short* data; channels} buffer descriptor is
// the SAudioBuffer of the landed tone stream Read 0x00C73F10 (same IAudioStream slot).
// Callee: libpk's vorbisfile ov_read @ 0x00C8BB20 (extern "C" __fastcall under the libpk LTCG link: vf in ecx, buffer in edx,
// the other five on the stack, as its first statement calls host_is_big_endian 0x00C8BB10).
// Model: the landed COggVorbisStream rows (0x00C88640) pasted verbatim, with the Read slot typed as the tone row types it.
// ---- libogg 1.1 / libvorbis 1.0.1 (20030909) public structures, as statically linked into libpk ----
typedef __int64 ogg_int64_t;

typedef struct {
    long endbyte;
    int endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long storage;
} oggpack_buffer;

typedef struct {
    unsigned char* data;
    int storage;
    int fill;
    int returned;
    int unsynced;
    int headerbytes;
    int bodybytes;
} ogg_sync_state;

typedef struct {
    unsigned char* body_data;
    long body_storage;
    long body_fill;
    long body_returned;
    int* lacing_vals;
    ogg_int64_t* granule_vals;
    long lacing_storage;
    long lacing_fill;
    long lacing_packet;
    long lacing_returned;
    unsigned char header[282];
    int header_fill;
    int e_o_s;
    int b_o_s;
    long serialno;
    long pageno;
    ogg_int64_t packetno;
    ogg_int64_t granulepos;
} ogg_stream_state;

typedef struct vorbis_info {
    int version;
    int channels;
    long rate;
    long bitrate_upper;
    long bitrate_nominal;
    long bitrate_lower;
    long bitrate_window;
    void* codec_setup;
} vorbis_info;

typedef struct vorbis_comment {
    char** user_comments;
    int* comment_lengths;
    int comments;
    char* vendor;
} vorbis_comment;

typedef struct vorbis_dsp_state {
    int analysisp;
    vorbis_info* vi;
    float** pcm;
    float** pcmret;
    int pcm_storage;
    int pcm_current;
    int pcm_returned;
    int preextrapolate;
    int eofflag;
    long lW;
    long W;
    long nW;
    long centerW;
    ogg_int64_t granulepos;
    ogg_int64_t sequence;
    ogg_int64_t glue_bits;
    ogg_int64_t time_bits;
    ogg_int64_t floor_bits;
    ogg_int64_t res_bits;
    void* backend_state;
} vorbis_dsp_state;

typedef struct vorbis_block {
    float** pcm;
    oggpack_buffer opb;
    long lW;
    long W;
    long nW;
    int pcmend;
    int mode;
    int eofflag;
    ogg_int64_t granulepos;
    ogg_int64_t sequence;
    vorbis_dsp_state* vd;
    void* localstore;
    long localtop;
    long localalloc;
    long totaluse;
    struct alloc_chain* reap;
    long glue_bits;
    long time_bits;
    long floor_bits;
    long res_bits;
    void* internal;
} vorbis_block;

// libpk's private vorbisfile: no ov_callbacks member (reads go through the owner's stream reader), 0x2C0 bytes.
typedef struct OggVorbis_File {
    void* datasource;                      // +0x000
    int seekable;                          // +0x004
    ogg_int64_t offset;                    // +0x008
    ogg_int64_t end;                       // +0x010
    ogg_sync_state oy;                     // +0x018
    int links;                             // +0x034
    ogg_int64_t* offsets;                  // +0x038
    ogg_int64_t* dataoffsets;              // +0x03c
    long* serialnos;                       // +0x040
    ogg_int64_t* pcmlengths;               // +0x044
    vorbis_info* vi;                       // +0x048
    vorbis_comment* vc;                    // +0x04c
    ogg_int64_t pcm_offset;                // +0x050
    int ready_state;                       // +0x058
    long current_serialno;                 // +0x05c
    int current_link;                      // +0x060
    double bittrack;                       // +0x068
    double samptrack;                      // +0x070
    ogg_stream_state os;                   // +0x078
    vorbis_dsp_state vd;                   // +0x1e0
    vorbis_block vb;                       // +0x250
} OggVorbis_File;

extern "C" ogg_int64_t __fastcall ov_pcm_total(OggVorbis_File* vf, int i);                // 0x00C8AA80
extern "C" int __fastcall ov_pcm_seek_page(OggVorbis_File* vf, ogg_int64_t pos);          // 0x00C8AE90
extern "C" int __fastcall ov_pcm_seek(OggVorbis_File* vf, ogg_int64_t pos);               // 0x00C8B490
extern "C" ogg_int64_t __fastcall ov_pcm_tell(OggVorbis_File* vf);                        // 0x00C8B9B0
extern "C" vorbis_info* __fastcall ov_info(OggVorbis_File* vf, int link);                 // 0x00C8BAB0

struct SStreamSegment {
    unsigned long m_word0;
    unsigned long m_word4;
    unsigned long m_word8;
};

class PKCBufferedReader {                  // vtable 0x012B8458 (.\PKCBufferedReader.cpp: the constructor's and seek's messages)
public:
    PKCBufferedReader(void* source, const SStreamSegment& segment);   // 0x00C4B300
    ~PKCBufferedReader();                                        // 0x00C4B270
    virtual void v00();
private:
    unsigned char _body04[0x6c];
};

struct SAudioBuffer {                    // the interleaved 16-bit block a stream's Read fills (landed 0x00C73F10)
    short*        m_data;                  // +0x00
    unsigned long m_channels;              // +0x04
};

class IAudioStream {                       // vtable 0x012BEFFC
public:
    virtual ~IAudioStream() {}
    virtual long GetLength() = 0;
    virtual long Seek(unsigned long position, bool exact) = 0;
    virtual long Read(SAudioBuffer* buffer, unsigned long frames) = 0;
    virtual long GetRate() = 0;
    virtual long GetState() = 0;
};

class COggVorbisStream : public IAudioStream {   // vtable 0x012BF1C4
public:
    COggVorbisStream(void* source, const SStreamSegment& segment);
    virtual ~COggVorbisStream();                  // 0x00C885C0
    virtual long GetLength();                     // 0x00C88620
    virtual long Seek(unsigned long position, bool exact);   // 0x00C88640
    virtual long Read(SAudioBuffer* buffer, unsigned long frames);   // 0x00C886B0
    virtual long GetRate();                       // 0x00C88740
    virtual long GetState();                      // 0x00C88770
private:
    PKCBufferedReader m_reader;                   // +0x04
    long m_valid;                                 // +0x74
    long m_opened;                                // +0x78
    OggVorbis_File m_vf;                          // +0x80
};

extern "C" long __fastcall ov_read(OggVorbis_File* vf, char* buffer, int length, int bigendianp, int word, int sgned, int* bitstream);   // 0x00C8BB20

long COggVorbisStream::Read(SAudioBuffer* buffer, unsigned long frames)
{
    if (!m_valid)
        return -1;
    if (frames == 0)
        return 0;
    char* out = (char*)buffer->m_data;
    int remaining = buffer->m_channels * frames * 2;
    unsigned long done = 0;
    int bitstream;
    for (;;) {
        long got = ov_read(&m_vf, out, remaining, 0, 2, 1, &bitstream);
        if (got <= 0) {
            if (got < 0)
                return got;
            break;
        }
        done += got;
        out += got;
        remaining -= got;
        if (remaining == 0)
            break;
    }
    return done / (buffer->m_channels * 2);
}