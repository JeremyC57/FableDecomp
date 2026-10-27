// libogg::ogg_stream_reset_serialno(ogg_stream_state* os, int serialno) -- Xiph framing.c: reset the stream state and give it a new serial number
// Grade: DIFFER(22v14) -- retail keeps os/serialno in ecx/edx across ogg_stream_reset (LTCG knows the callee preserves them); VC7.1 saves them in esi/edi (lane gap: LTCG)
// Leaf: @ogg_stream_reset_serialno@8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8D9C0, 14 bytes, __fastcall (os ecx, serialno edx), plain ret.
//   Callee: ogg_stream_reset 0x00C8D970 (landed @ogg_stream_reset@4); LTCG knows it leaves ecx/edx intact, so retail keeps
//   os and serialno in ecx/edx across the call.
// Library: libpk statically links the Xiph Ogg Vorbis decoder ("Xiph.Org libVorbis I 20030909" @0x012BF240; libogg 1.1 /
//   libvorbis post-1.0.1 CVS), compiled as C with /Gr: the functions are extern "C" __fastcall.
// Identity: framing.c's ogg_stream_reset_serialno verbatim (serialno +0x150, returns 0); eight vorbisfile.c callers
//   (0x00C8A102..0x00C8B668). Label correction: the catalog's "CRegionMinimap_CRegionMinimap" is propagated.
typedef __int64 ogg_int64_t;
typedef struct {
    unsigned char* body_data;
    long body_storage;
    long body_fill;
    long body_returned;
    int* lacing_vals;
    ogg_int64_t* granule_vals;
    long lacing_storage;
    long lacing_fill;
    long lacing_packet;
    long lacing_returned;
    unsigned char header[282];
    int header_fill;
    int e_o_s;
    int b_o_s;
    long serialno;                      // +0x150
    long pageno;
    ogg_int64_t packetno;
    ogg_int64_t granulepos;
} ogg_stream_state;

extern "C" int __fastcall ogg_stream_reset(ogg_stream_state* os);   // 0x00C8D970

extern "C" int __fastcall ogg_stream_reset_serialno(ogg_stream_state* os, int serialno)
{
    ogg_stream_reset(os);
    os->serialno = serialno;
    return 0;
}
