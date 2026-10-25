// libvorbis::ov_time_total(OggVorbis_File* vf, int i) -- vorbisfile: playing time in seconds of logical bitstream i, or of the whole file (sum over links) when i < 0; OV_EINVAL when not opened, unseekable or i out of range
// Leaf: @ov_time_total@8
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8AAF0, length 128 bytes.
// Library: libpk's statically linked Xiph vorbisfile (the exe links "Xiph.Org libVorbis I 20030909", string
// @ 0x012BF240, used by _vorbis_pack_comment 0x00C8E790). No PDB/RTTI: the name is the libvorbis API name.
// Convention: the LTCG link turned the static-linked C API into __fastcall (vf in ecx, i in edx, callee-clean);
// the recursion `call 0x00C8AAF0` with `mov edx,edi ; mov ecx,esi` proves it. Constants: 0.0 @ 0x0122ED70,
// (double)OV_EINVAL = -131.0 @ 0x012BF218.
// Shape: the vorbisfile 1.0.x body verbatim. The double accumulator gets an 8-aligned frame (`and esp,-8`); the
// sum is `call ; fadd [acc]`. The per-link time is `(double)pcmlengths[i*2+1] / vi[i].rate` (`fild qword` of the
// end pcm offset at pcmlengths + i*16 + 8, `fidiv dword` of vorbis_info::rate at vi + i*32 + 8).
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

extern "C" double __fastcall ov_time_total(OggVorbis_File* vf, int i)
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