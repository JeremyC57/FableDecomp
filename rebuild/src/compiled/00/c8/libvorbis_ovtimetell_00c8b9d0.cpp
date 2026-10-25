// libvorbis::ov_time_tell(OggVorbis_File* vf) -- vorbisfile: current playback time in seconds: walk the links back from the last until the pcm offset falls inside one, then add that link's start time and the in-link sample offset over its rate
// Leaf: @ov_time_tell@4
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8B9D0, length 212 bytes (224 with padding).
// Library: libpk's statically linked Xiph vorbisfile (libVorbis I 20030909); no PDB/RTTI, libvorbis API name.
// Convention: LTCG __fastcall (vf in ecx). Callees, both LTCG __fastcall (vf in ecx, link in edx):
//   ov_pcm_total 0x00C8AA80 (ogg_int64_t in edx:eax), ov_time_total 0x00C8AAF0 (double in st0).
// Constants: 0.0 @ 0x0122ED70, (double)OV_EINVAL = -131.0 @ 0x012BF218.
// Shape: the vorbisfile 1.0.x body, with ov_time_total's real body (0x00C8AAF0, matched) in the same TU under
// auto_inline(off) + noinline: VC7.1 then sees the callee writes no memory and hoists `&vf->pcmlengths[link*2+1]`
// into ebx across the loop calls (retail's LTCG knowledge); with an extern callee it reloads the base each pass.
// The `time_total = 0.f` init is stored before the ready_state test; the 64-bit pcm_total lives in
// [esp+0x10]:ebp, `time_total -= ov_time_total(vf, link)` is `fsubr [time_total]`, and the break test
// `vf->pcm_offset >= pcm_total` is the hi-then-lo `jg/jl/jae` compare.
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

#define OV_EINVAL -131
#define OPENED    2

extern "C" ogg_int64_t __fastcall ov_pcm_total(OggVorbis_File* vf, int i);   // 0x00C8AA80
#pragma auto_inline(off)
extern "C" __declspec(noinline) double __fastcall ov_time_total(OggVorbis_File* vf, int i)   // 0x00C8AAF0 (same-TU body)
{
    if (vf->ready_state < OPENED) return (OV_EINVAL);
    if (!vf->seekable || i >= vf->links) return (OV_EINVAL);
    if (i < 0) {
        double acc = 0;
        int i;
        for (i = 0; i < vf->links; i++)
            acc += ov_time_total(vf, i);
        return (acc);
    } else {
        return ((double)(vf->pcmlengths[i * 2 + 1]) / vf->vi[i].rate);
    }
}
#pragma auto_inline(on)

extern "C" double __fastcall ov_time_tell(OggVorbis_File* vf)
{
    int link = 0;
    ogg_int64_t pcm_total = 0;
    double time_total = 0.f;

    if (vf->ready_state < OPENED) return (OV_EINVAL);
    if (vf->seekable) {
        pcm_total = ov_pcm_total(vf, -1);
        time_total = ov_time_total(vf, -1);

        /* which bitstream section does this time offset occur in? */
        for (link = vf->links - 1; link >= 0; link--) {
            pcm_total -= vf->pcmlengths[link * 2 + 1];
            time_total -= ov_time_total(vf, link);
            if (vf->pcm_offset >= pcm_total) break;
        }
    }

    return ((double)time_total + (double)(vf->pcm_offset - pcm_total) / vf->vi[link].rate);
}