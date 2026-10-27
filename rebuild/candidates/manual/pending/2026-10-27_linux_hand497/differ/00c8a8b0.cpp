// libvorbis::ov_halfrate(OggVorbis_File* vf, int flag) -- Xiph vorbisfile.c: switch every link's decoder in or out of half-rate mode (OV_EINVAL without info or when unseekable; the decode state is cleared first; on a refusal it switches back to full rate and fails)
// Grade: DIFFER(107v105) -- retail calls the static _decode_clear 0x00C8A520 with vf already in esi (LTCG register argument: its body reads esi); a __fastcall declaration adds `mov ecx,esi` (lane gap: LTCG)
// Leaf: @ov_halfrate@8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8A8B0, 105 bytes, __fastcall (vf ecx, flag edx), plain ret.
// Callees: _decode_clear 0x00C8A520 (static), vorbis_synthesis_halfrate 0x00C8F030, itself (ov_halfrate(vf,0)).
// Library: libpk statically links the Xiph Ogg Vorbis decoder ("Xiph.Org libVorbis I 20030909" @0x012BF240; libpk fork of
//   vorbisfile.c without ov_callbacks, compiled /Gr: the API is extern "C" __fastcall, as the landed ov_* rows spell it).
// Identity: the vorbisfile.c 1.0.x+ body verbatim (vi +0x48, seekable +4, ready_state +0x58 >= STREAMSET, links +0x34,
//   vi + i scaled by sizeof(vorbis_info) 0x20); its only caller is itself (0x00C8A90A); it precedes ov_halfrate_p
//   0x00C8A920 as in the Xiph source. The catalog label (gap-scan) is a placeholder.
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
extern "C" int __fastcall vorbis_synthesis_halfrate(vorbis_info* vi, int flag);   // 0x00C8F030
extern "C" void __fastcall _decode_clear(OggVorbis_File* vf);                    // 0x00C8A520 (static in vorbisfile.c)

extern "C" int __fastcall ov_halfrate(OggVorbis_File* vf, int flag)
{
    int i;
    if (vf->vi == 0) return OV_EINVAL;
    if (!vf->seekable) return OV_EINVAL;
    if (vf->ready_state >= STREAMSET)
        _decode_clear(vf);

    for (i = 0; i < vf->links; i++) {
        if (vorbis_synthesis_halfrate(vf->vi + i, flag)) {
            ov_halfrate(vf, 0);
            return OV_EINVAL;
        }
    }
    return 0;
}
