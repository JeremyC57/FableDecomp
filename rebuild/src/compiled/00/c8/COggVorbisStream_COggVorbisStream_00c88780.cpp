// COggVorbisStream::COggVorbisStream(void* source, const SStreamSegment& segment) -- build the stream reader over the source segment, open it as an Ogg Vorbis file, and mark the stream valid only when it is seekable and every logical link is stereo
// Flags: /EHsc
// Leaf: ??0COggVorbisStream@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C88780, length 237 bytes (+ int3 padding), ret 8. EH frame (handler
//   0x01216870, unwind map 0x01370EA0: state 0 = the interface base, state 1 = the reader member).
// Callees: CStreamReader ctor 0x00C4B300 (dtor 0x00C4B270), libpk's vorbisfile ov_open 0x00C8CAE0, ov_seekable
//   0x00C8A950, ov_comment 0x00C8BAE0, ov_info 0x00C8BAB0 (all __fastcall).
// Layout (descriptive; libpk Ogg decoder, no PDB/RTTI): vtable 0x012BF1C4 (deleting dtor 0x00C88870, then GetLength
//   0x00C88620 = ov_pcm_total(-1), Seek 0x00C88640, Read 0x00C886B0, GetRate 0x00C88740, GetState 0x00C88770);
//   interface base vtable 0x012BEFFC (inline dtor 0x00C88890). Reader +4 (vtable 0x012B8458, 0x70 bytes), valid
//   flag +0x74, opened flag +0x78 (the dtor 0x00C885C0 calls ov_clear when set), OggVorbis_File +0x80 (0x2C0 bytes).
// Shape: the vorbisfile entry points are declared with C++ linkage: under /EHsc an extern "C" callee is nothrow, which
//   drops the state-1 store before ov_open. `vc` / `vi` are named locals (the header info read is otherwise unused)
//   and the comment walk is an empty `for`: this is what gives retail's ebp (reader, then comment cursor, then link
//   index) / edi (file) assignment; the one-expression spelling swaps the two registers.
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

int __fastcall ov_open(void* datasource, OggVorbis_File* vf, char* initial, long ibytes);   // 0x00C8CAE0
long __fastcall ov_seekable(OggVorbis_File* vf);                                            // 0x00C8A950
vorbis_comment* __fastcall ov_comment(OggVorbis_File* vf, int link);                        // 0x00C8BAE0
vorbis_info* __fastcall ov_info(OggVorbis_File* vf, int link);                              // 0x00C8BAB0

struct SStreamSegment {
    unsigned long m_word0;
    unsigned long m_word4;
    unsigned long m_word8;
};

class CStreamReader {                      // vtable 0x012B8458
public:
    CStreamReader(void* source, const SStreamSegment& segment);   // 0x00C4B300
    ~CStreamReader();                                            // 0x00C4B270
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
    CStreamReader m_reader;                       // +0x04
    long m_valid;                                 // +0x74
    long m_opened;                                // +0x78
    OggVorbis_File m_vf;                          // +0x80
};

COggVorbisStream::COggVorbisStream(void* source, const SStreamSegment& segment)
    : m_reader(source, segment)
{
    m_valid = 0;
    m_opened = 1;
    if (ov_open(&m_reader, &m_vf, 0, 0) < 0)
        return;
    m_opened = 1;
    if (!ov_seekable(&m_vf))
        return;
    vorbis_comment* vc = ov_comment(&m_vf, -1);
    char** comment = vc->user_comments;
    vorbis_info* vi = ov_info(&m_vf, -1);
    for (; *comment; ++comment) {
    }
    int i;
    for (i = 0; i < m_vf.links; i++) {
        vi = ov_info(&m_vf, i);
        if (vi->channels != 2)
            return;
    }
    m_valid = 1;
}