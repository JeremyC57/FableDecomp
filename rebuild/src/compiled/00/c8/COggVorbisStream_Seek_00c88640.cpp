// COggVorbisStream::Seek(unsigned long position, bool exact) -- seek the decoder to a PCM frame: nothing to do when already there; else ov_pcm_seek (exact) or ov_pcm_seek_page; returns the new position, or -1 when invalid or the seek fails
// Leaf: ?Seek@COggVorbisStream@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C88640, 101 bytes, thiscall, ret 8. Callees: ov_pcm_tell 0x00C8B9B0
//   (twice), ov_pcm_seek 0x00C8B490 when exact, else ov_pcm_seek_page 0x00C8AE90.
// Identity: vtable 0x012BF1C4 of the landed COggVorbisStream (ctor 0x00C88780, which documents the slots: GetLength
//   0x00C88620 = ov_pcm_total(-1), Seek 0x00C88640, Read 0x00C886B0, GetRate 0x00C88740, GetState 0x00C88770); valid flag
//   +0x74, OggVorbis_File +0x80. libpk has no PDB/RTTI: descriptive class/slot names (agent446/sweep1); the ov_* callees
//   are the Xiph vorbisfile API (extern "C" __fastcall under the libpk link, as their landed rows spell them).
//   ov_pcm_seek_page / ov_pcm_seek: vorbisfile.c's order after ov_time_total 0x00C8AAF0 is ov_raw_seek 0x00C8AB70,
//   ov_pcm_seek_page 0x00C8AE90, ov_pcm_seek 0x00C8B490, ov_time_seek 0x00C8B770, ov_time_seek_page 0x00C8B880.
// Shape: the unsigned position widens to a 64-bit value with a zero high word (edi) shared by the compare and the pushes.
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

class IAudioStream {                       // vtable 0x012BEFFC
public:
    virtual ~IAudioStream() {}
    virtual long GetLength() = 0;
    virtual long Seek(unsigned long position, bool exact) = 0;
    virtual long Read(void* buffer, unsigned long size) = 0;
    virtual long GetRate() = 0;
    virtual long GetState() = 0;
};

class COggVorbisStream : public IAudioStream {   // vtable 0x012BF1C4
public:
    COggVorbisStream(void* source, const SStreamSegment& segment);
    virtual ~COggVorbisStream();                  // 0x00C885C0
    virtual long GetLength();                     // 0x00C88620
    virtual long Seek(unsigned long position, bool exact);   // 0x00C88640
    virtual long Read(void* buffer, unsigned long size);     // 0x00C886B0
    virtual long GetRate();                       // 0x00C88740
    virtual long GetState();                      // 0x00C88770
private:
    PKCBufferedReader m_reader;                   // +0x04
    long m_valid;                                 // +0x74
    long m_opened;                                // +0x78
    OggVorbis_File m_vf;                          // +0x80
};

long COggVorbisStream::Seek(unsigned long position, bool exact)
{
    if (!m_valid)
        return -1;
    if (ov_pcm_tell(&m_vf) == position)
        return position;
    int result;
    if (exact)
        result = ov_pcm_seek(&m_vf, position);
    else
        result = ov_pcm_seek_page(&m_vf, position);
    if (result < 0)
        return -1;
    return (long)ov_pcm_tell(&m_vf);
}