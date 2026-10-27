// libvorbis::vorbis_synthesis_halfrate(vorbis_info* vi, int flag) -- Xiph synthesis.c: set or clear the codec setup's half-rate decode flag; refused (-1) when asked for half rate with 64-sample short blocks
// Leaf: @vorbis_synthesis_halfrate@8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8F030, 32 bytes, __fastcall (vi ecx, flag edx), plain ret. No callees.
// Library: libpk statically links the Xiph Ogg Vorbis decoder ("Xiph.Org libVorbis I 20030909"); compiled /Gr, the API
//   is extern "C" __fastcall.
// Identity: the synthesis.c body verbatim (codec_setup +0x1c, blocksizes[0] +0, halfrate_flag +0xE78 as the landed
//   vorbis_synthesis_halfrate_p 0x00C8F050 that follows it reads); its caller is ov_halfrate 0x00C8A8B0 (0x00C8A8EB).
//   The catalog label (gap-scan) is a placeholder.
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
struct codec_setup_info {               // 0xE80 bytes
    long          blocksizes[2];        // +0x000
    unsigned char _body[0xe78 - 8];
    int           halfrate_flag;        // +0xE78
    unsigned char _tail[0xe80 - 0xe7c];
};

extern "C" int __fastcall vorbis_synthesis_halfrate(vorbis_info* vi, int flag)
{
    /* set / clear half-sample-rate mode */
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;

    /* right now, our MDCT can't handle < 64 sample windows. */
    if (ci->blocksizes[0] <= 64 && flag) return -1;
    ci->halfrate_flag = (flag ? 1 : 0);
    return 0;
}