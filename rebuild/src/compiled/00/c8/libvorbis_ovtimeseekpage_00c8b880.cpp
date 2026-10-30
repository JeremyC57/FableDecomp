// libvorbis::ov_time_seek_page(OggVorbis_File* vf, double seconds) -- vorbisfile: page-granularity time seek: translate a time offset in seconds to a pcm position (walk the links back from the last until the time falls inside one) and seek there with ov_pcm_seek_page; OV_EINVAL when not opened or out of range, OV_ENOSEEK when unseekable
// Leaf: @ov_time_seek_page@12
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8B880, length 257 bytes (the byte twin of ov_time_seek 0x00C8B770
// apart from the tail call).
// Library: libpk's statically linked Xiph vorbisfile (libVorbis I 20030909, string @ 0x012BF240); no PDB/RTTI, so the
// name is the libvorbis API name. Identity: the body is vorbisfile.c 1.0.x ov_time_seek_page verbatim (OV_ENOSEEK -138 /
// OV_EINVAL -131 returns, the pcmlengths[link*2+1] / ov_time_total walk) and its tail calls ov_pcm_seek_page 0x00C8AE90
// (bound @ov_pcm_seek_page@12); the twin just before it, 0x00C8B770, calls ov_pcm_seek 0x00C8B490 = ov_time_seek, which
// matches the vorbisfile.c source order (ov_time_seek, then ov_time_seek_page).
// Convention: LTCG __fastcall (vf in ecx, the double on the stack, `ret 8`), like every libpk ov_* row.
// Callees: ov_pcm_total 0x00C8AA80 and ov_time_total 0x00C8AAF0 (__fastcall, vf in ecx, link in edx; ov_time_total
// is landed and its real body sits in this TU under auto_inline(off) + noinline, as in the landed ov_time_tell
// 0x00C8B9D0, so VC7.1 sees it writes no memory and keeps &pcmlengths[link*2+1] in ebx across the loop calls),
// ov_pcm_seek_page 0x00C8AE90, __ftol2 0x00BFEA70 for the ogg_int64_t conversion. Constant: 0.0 @ 0x0122ED70.
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
struct vorbis_comment;
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

#define OV_EINVAL  -131
#define OV_ENOSEEK -138
#define OPENED     2

extern "C" ogg_int64_t __fastcall ov_pcm_total(OggVorbis_File* vf, int i);        // 0x00C8AA80
extern "C" int __fastcall ov_pcm_seek_page(OggVorbis_File* vf, ogg_int64_t pos);  // 0x00C8AE90
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

extern "C" int __fastcall ov_time_seek_page(OggVorbis_File* vf, double seconds)
{
    /* translate time to PCM position and call ov_pcm_seek_page */

    int link = -1;
    ogg_int64_t pcm_total = ov_pcm_total(vf, -1);
    double time_total = ov_time_total(vf, -1);

    if (vf->ready_state < OPENED) return (OV_EINVAL);
    if (!vf->seekable) return (OV_ENOSEEK);
    if (seconds < 0 || seconds > time_total) return (OV_EINVAL);

    /* which bitstream section does this time offset occur in? */
    for (link = vf->links - 1; link >= 0; link--) {
        pcm_total -= vf->pcmlengths[link * 2 + 1];
        time_total -= ov_time_total(vf, link);
        if (seconds >= time_total) break;
    }

    /* enough information to convert time offset to pcm offset */
    {
        ogg_int64_t target = pcm_total + (seconds - time_total) * vf->vi[link].rate;
        return (ov_pcm_seek_page(vf, target));
    }
}