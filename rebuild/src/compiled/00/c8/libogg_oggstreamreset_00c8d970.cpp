// libogg::ogg_stream_reset(ogg_stream_state* os) -- Xiph libogg framing.c: rewind a 0x168-byte ogg_stream_state: clear its fill and lacing counters, header fill, e_o_s/b_o_s, pageno = -1, packetno and granulepos = 0; returns 0
// Leaf: @ogg_stream_reset@4
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8D970, LTCG `__fastcall` (the Xiph API takes ecx/edx under the libpk link).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 446 "renames only" list): the Xiph Ogg Vorbis decoder libpk links statically
//   ("Xiph.Org libVorbis I 20030909", agent446 NOTES.md); its thirteen stores are exactly the ogg_stream_state fields framing.c's ogg_stream_reset clears (body_fill +8, body_returned +0xc, lacing_fill/packet/returned +0x1c..+0x24, header_fill +0x144, e_o_s +0x148, b_o_s +0x14c, pageno = -1 +0x154, packetno +0x158, granulepos +0x160) over the 0x168-byte record ogg_stream_init 0x00C8CE20 zero-fills; returns 0 in eax.
//   Real source name (the Xiph API), not a descriptive one.
// Label correction: the landed row was SBufferedStreamState00c8d970::Reset, a propagated game-side name over an invented model.
typedef __int64 ogg_int64_t;
struct ogg_stream_state {               // 0x168 bytes
    unsigned char* body_data;           // +0x000
    long           body_storage;        // +0x004
    long           body_fill;           // +0x008
    long           body_returned;       // +0x00c
    int*           lacing_vals;         // +0x010
    ogg_int64_t*   granule_vals;        // +0x014
    long           lacing_storage;      // +0x018
    long           lacing_fill;         // +0x01c
    long           lacing_packet;       // +0x020
    long           lacing_returned;     // +0x024
    unsigned char  header[282];         // +0x028
    int            header_fill;         // +0x144
    int            e_o_s;               // +0x148
    int            b_o_s;               // +0x14c
    long           serialno;            // +0x150
    long           pageno;              // +0x154
    ogg_int64_t    packetno;            // +0x158
    ogg_int64_t    granulepos;          // +0x160
};

extern "C" int __fastcall ogg_stream_reset(ogg_stream_state* os)
{
    os->body_fill = 0;
    os->body_returned = 0;

    os->lacing_fill = 0;
    os->lacing_packet = 0;
    os->lacing_returned = 0;

    os->header_fill = 0;

    os->e_o_s = 0;
    os->b_o_s = 0;
    os->pageno = -1;
    os->packetno = 0;
    os->granulepos = 0;

    return (0);
}
