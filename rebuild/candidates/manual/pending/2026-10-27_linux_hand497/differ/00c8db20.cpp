// libogg::ogg_stream_packetpeek(ogg_stream_state* os, ogg_packet* op) -- Xiph framing.c: look at the next packet without consuming it (_packetout with adv = 0)
// Grade: DIFFER(8v12) -- retail calls the static _packetout 0x00C8D9D0 with LTCG registers (os in eax, op in ecx, adv pushed); a __fastcall declaration becomes push 0 + tail call (lane gap: LTCG)
// Leaf: @ogg_stream_packetpeek@8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8DB20, 12 bytes, __fastcall (os ecx, op edx), plain ret.
//   Callee: _packetout 0x00C8D9D0 (static; LTCG: os in eax, op in ecx, adv on the stack).
// Library: libpk statically links the Xiph Ogg Vorbis decoder ("Xiph.Org libVorbis I 20030909" @0x012BF240; libogg 1.1 /
//   libvorbis post-1.0.1 CVS), compiled as C with /Gr: the functions are extern "C" __fastcall.
// Identity: framing.c's ogg_stream_packetpeek verbatim; it sits right after ogg_stream_packetout 0x00C8DB10 (adv = 1)
//   and before ogg_packet_clear 0x00C8DB30, the framing.c order; callers in vorbisfile.c 0x00C8B2C0, 0x00C8B30C,
//   0x00C8B4CA. Label correction: the catalog's "_Dest_val<std::allocator<NCreatureHitNotification::CFlash>,...>" is propagated.
typedef struct ogg_stream_state ogg_stream_state;
typedef struct ogg_packet ogg_packet;
extern "C" int __fastcall _packetout(ogg_stream_state* os, ogg_packet* op, int adv);   // 0x00C8D9D0 (static)

extern "C" int __fastcall ogg_stream_packetpeek(ogg_stream_state* os, ogg_packet* op)
{
    return _packetout(os, op, 0);
}
