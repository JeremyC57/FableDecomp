// ov_seekable(OggVorbis_File* vf) -- Xiph vorbisfile.c: whether the stream is seekable (vf->seekable, +0x04)
// Leaf: @ov_seekable@4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8A950, LTCG `__fastcall` (the Xiph API takes ecx/edx under the libpk link).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 446 "renames only" list): the Xiph Ogg Vorbis decoder libpk links statically
//   ("Xiph.Org libVorbis I 20030909", agent446 NOTES.md); `mov eax,[ecx+4] ; ret` reads OggVorbis_File::seekable; it follows ov_halfrate_p 0x00C8A920 as in vorbisfile.c (ov_halfrate_p, ov_seekable, ov_streams order of the 1.0.x CVS tree).
//   Real source name (the Xiph API), not a descriptive one.
// Label correction: the landed row was global DrawGetEnvironment (a `__fastcall` +4 reader), a propagated game-side name over an invented model.
typedef __int64 ogg_int64_t;
struct vorbis_info;
struct vorbis_comment;
struct ogg_sync_state {                 // 0x1c bytes
    unsigned char* data;
    int storage, fill, returned, unsynced, headerbytes, bodybytes;
};
struct OggVorbis_File {                 // libpk's fork: no ov_callbacks member
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

extern "C" long __fastcall ov_seekable(OggVorbis_File* vf)
{
    return vf->seekable;
}
