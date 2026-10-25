// vorbis_synthesis_halfrate_p(vorbis_info* vi) -- Xiph libvorbis synthesis.c: the codec setup's halfrate_flag
// Leaf: @vorbis_synthesis_halfrate_p@4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8F050, LTCG `__fastcall` (the Xiph API takes ecx/edx under the libpk link).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 446 "renames only" list): the Xiph Ogg Vorbis decoder libpk links statically
//   ("Xiph.Org libVorbis I 20030909", agent446 NOTES.md); return vi->codec_setup->halfrate_flag (+0x1c, then +0xE78: agent446 places codec_setup_info::halfrate_flag at +0xE78); ov_halfrate_p 0x00C8A920 tail-calls it on vf->vi, as vorbisfile.c does; it follows vorbis_packet_blocksize 0x00C8EFC0 (agent446, synthesis.c order).
//   Real source name (the Xiph API), not a descriptive one.
// Label correction: the landed row was placeholder C_00c8f050::M (CountReferences), a propagated game-side name over an invented model.
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
    unsigned char _head[0xe78];
    int           halfrate_flag;        // +0xE78
    unsigned char _tail[0xe80 - 0xe7c];
};

extern "C" int __fastcall vorbis_synthesis_halfrate_p(vorbis_info* vi)
{
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
    return ci->halfrate_flag;
}
