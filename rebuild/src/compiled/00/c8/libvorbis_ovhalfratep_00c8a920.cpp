// libvorbis::ov_halfrate_p(OggVorbis_File* vf) -- Xiph vorbisfile.c: OV_EINVAL (-131) without decoder info, else vorbis_synthesis_halfrate_p(vf->vi)
// Leaf: @ov_halfrate_p@4
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8A920, LTCG `__fastcall` (the Xiph API takes ecx/edx under the libpk link).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 446 "renames only" list): the Xiph Ogg Vorbis decoder libpk links statically
//   ("Xiph.Org libVorbis I 20030909", agent446 NOTES.md); vf->vi (+0x48 of OggVorbis_File) null gives -131 (OV_EINVAL), else the tail call to vorbis_synthesis_halfrate_p 0x00C8F050 -- the vorbisfile.c body; the landed row reached +0x48 through the engine CWorldMap header (a wrong class) and named the callee CRegionRouterCalls::Route.
//   Real source name (the Xiph API), not a descriptive one.
// Label correction: the landed row was CWorldMap::GetNextRegionOnRouteTo (through engine/CWorldMap.h), a propagated game-side name over an invented model.
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
extern "C" int __fastcall vorbis_synthesis_halfrate_p(vorbis_info* vi);   // 0x00C8F050

extern "C" int __fastcall ov_halfrate_p(OggVorbis_File* vf)
{
    if (vf->vi == 0) return OV_EINVAL;
    return vorbis_synthesis_halfrate_p(vf->vi);
}
