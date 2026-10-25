// libvorbis::ov_info(OggVorbis_File* vf, int link) -- vorbisfile: the vorbis_info of logical bitstream `link` (the current link when link < 0 and a stream is set, the first otherwise; NULL past the last link; always the first when unseekable)
// Leaf: @ov_info@8
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8BAB0, length 43 bytes (48 with padding).
// Library: libpk's statically linked Xiph vorbisfile (libVorbis I 20030909); no PDB/RTTI, libvorbis API name.
// Convention: LTCG __fastcall (vf in ecx, link in edx; `mov eax,edx` frees edx for the seekable load). No callees.
// Shape: the vorbisfile 1.0.x nested if/else verbatim; `vf->vi + n` scales by sizeof(vorbis_info) = 32 (`shl 5`),
// and the two `vi + n` returns share one `mov edx,[ecx+48h] ; shl ; add` tail.
typedef __int64 ogg_int64_t;

struct vorbis_info {                    // 0x20 bytes
    int   version;                      // +0x00
    int   channels;                     // +0x04
    long  rate;                         // +0x08
    long  bitrate_upper;                // +0x0c
    long  bitrate_nominal;              // +0x10
    long  bitrate_lower;                // +0x14
    long  bitrate_window;               // +0x18
    void* codec_setup;                  // +0x1c
};
struct vorbis_comment {                 // 0x10 bytes
    char** user_comments;               // +0x00
    int*   comment_lengths;             // +0x04
    int    comments;                    // +0x08
    char*  vendor;                      // +0x0c
};
struct ogg_sync_state {                 // 0x1c bytes
    unsigned char* data;
    int storage, fill, returned, unsynced, headerbytes, bodybytes;
};
struct OggVorbis_File {
    void*           datasource;         // +0x00
    int             seekable;           // +0x04
    ogg_int64_t     offset;             // +0x08
    ogg_int64_t     end;                // +0x10
    ogg_sync_state  oy;                 // +0x18
    int             links;              // +0x34
    ogg_int64_t*    offsets;            // +0x38
    ogg_int64_t*    dataoffsets;        // +0x3c
    long*           serialnos;          // +0x40
    ogg_int64_t*    pcmlengths;         // +0x44 (two per link: begin, end)
    vorbis_info*    vi;                 // +0x48 (one per link)
    vorbis_comment* vc;                 // +0x4c (one per link)
    ogg_int64_t     pcm_offset;         // +0x50
    int             ready_state;        // +0x58
    long            current_serialno;   // +0x5c
    int             current_link;       // +0x60
    double          bittrack;           // +0x68
    double          samptrack;          // +0x70
};

#define STREAMSET 3

extern "C" vorbis_info* __fastcall ov_info(OggVorbis_File* vf, int link)
{
    if (vf->seekable) {
        if (link < 0)
            if (vf->ready_state >= STREAMSET)
                return vf->vi + vf->current_link;
            else
                return vf->vi;
        else
            if (link >= vf->links)
                return 0;
            else
                return vf->vi + link;
    } else {
        return vf->vi;
    }
}