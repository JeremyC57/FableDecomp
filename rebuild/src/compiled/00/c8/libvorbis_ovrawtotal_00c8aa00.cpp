// libvorbis::ov_raw_total(OggVorbis_File* vf, int i) -- Xiph vorbisfile.c: the compressed byte length of link i, or of the whole file when i < 0 (OV_EINVAL before opened, when unseekable or past the last link)
// Leaf: @ov_raw_total@8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8AA00, 114 bytes, __fastcall (vf ecx, i edx), plain ret, 64-bit result.
// Callee: itself (the per-link sum).
// Library: libpk statically links the Xiph Ogg Vorbis decoder ("Xiph.Org libVorbis I 20030909" @0x012BF240; libpk fork of
//   vorbisfile.c without ov_callbacks, compiled /Gr: the API is extern "C" __fastcall, as the landed ov_* rows spell it).
// Identity: the vorbisfile.c body verbatim (ready_state +0x58 < OPENED, seekable +4, links +0x34, offsets +0x38:
//   offsets[i+1]-offsets[i]); its only caller is itself (0x00C8AA34); it precedes ov_pcm_total 0x00C8AA80, whose landed
//   body has the same shape. The catalog label (gap-scan) is a placeholder.
typedef __int64 ogg_int64_t;
struct vorbis_info {                    // 0x20 bytes
    int   version;                      // +0x00
    int   channels;                     // +0x04
    long  rate;                         // +0x08
    long  bitrate_upper;                // +0x0c
    long  bitrate_nominal;              // +0x10
    long  bitrate_lower;                // +0x14
    long  bitrate_window;               // +0x18
    void* codec_setup;                  // +0x1c (codec_setup_info, 0xE80 bytes)
};
struct vorbis_comment;
struct ogg_sync_state {                 // 0x1c bytes
    unsigned char* data;
    int storage, fill, returned, unsynced, headerbytes, bodybytes;
};
struct OggVorbis_File {                 // libpk's fork: no ov_callbacks member (0x2C0 bytes; the decoder state follows)
    void*           datasource;         // +0x00
    int             seekable;           // +0x04
    ogg_int64_t     offset;             // +0x08
    ogg_int64_t     end;                // +0x10
    ogg_sync_state  oy;                 // +0x18
    int             links;              // +0x34
    ogg_int64_t*    offsets;            // +0x38
    ogg_int64_t*    dataoffsets;        // +0x3c
    long*           serialnos;          // +0x40
    ogg_int64_t*    pcmlengths;         // +0x44
    vorbis_info*    vi;                 // +0x48
    vorbis_comment* vc;                 // +0x4c
    ogg_int64_t     pcm_offset;         // +0x50
    int             ready_state;        // +0x58
    long            current_serialno;   // +0x5c
    int             current_link;       // +0x60
    double          bittrack;           // +0x68
    double          samptrack;          // +0x70
};

#define OV_EINVAL -131
#define OPENED    2
#define STREAMSET 3

extern "C" ogg_int64_t __fastcall ov_raw_total(OggVorbis_File* vf, int i)
{
    if (vf->ready_state < OPENED) return (OV_EINVAL);
    if (!vf->seekable || i >= vf->links) return (OV_EINVAL);
    if (i < 0) {
        ogg_int64_t acc = 0;
        int i;
        for (i = 0; i < vf->links; i++)
            acc += ov_raw_total(vf, i);
        return (acc);
    } else {
        return (vf->offsets[i + 1] - vf->offsets[i]);
    }
}