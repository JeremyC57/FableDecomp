// ov_pcm_tell(OggVorbis_File* vf) -- Xiph vorbisfile.c: OV_EINVAL before the file is opened, else the current PCM offset
// Leaf: @ov_pcm_tell@4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8B9B0, LTCG `__fastcall` (the Xiph API takes ecx/edx under the libpk link).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 446 "renames only" list): the Xiph Ogg Vorbis decoder libpk links statically
//   ("Xiph.Org libVorbis I 20030909", agent446 NOTES.md); ready_state (+0x58) < OPENED (2) returns -131 as a 64-bit value, else pcm_offset (+0x50) -- the vorbisfile.c body; it precedes ov_time_tell 0x00C8B9D0 (agent446).
//   Real source name (the Xiph API), not a descriptive one.
// Label correction: the landed row was CCategory_GetHighlightedItem, a propagated game-side name over an invented model.
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

extern "C" ogg_int64_t __fastcall ov_pcm_tell(OggVorbis_File* vf)
{
    if (vf->ready_state < OPENED) return (OV_EINVAL);
    return (vf->pcm_offset);
}
