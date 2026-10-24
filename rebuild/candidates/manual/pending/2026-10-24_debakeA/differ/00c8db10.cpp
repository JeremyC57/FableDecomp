// Grade: DIFFER(8v12) -- IMPOSSIBLE (LTCG custom convention): the row is libogg ogg_stream_packetout (framing.c `return _packetout(os, op, 1)`), not std::_Dest_val<CFlash> (label wrong). libogg was built with link-time code generation: the static callee _packetout 0x00C8D9D0 takes `os` in eax and `op` in ecx (`mov ebx,[eax+24h] ; mov esi,ecx`, `ret 4`), so retail shuffles `mov eax,ecx ; push 1 ; mov ecx,edx`. Plain VC7.1 has no way to pass an argument in eax (__fastcall gives ecx/edx: this form is `push 1 ; call ; ret`). Same family as _ov_d_seek_lap 0x00C8C730. One grade.
// ogg_stream_packetout(os, op) -- libogg framing.c: return _packetout(os, op, 1)
// Leaf: ogg_stream_packetout
struct ogg_stream_state;
struct ogg_packet;
int __fastcall _packetout(ogg_stream_state* os, ogg_packet* op, int adv);   // 0x00C8D9D0
int __fastcall ogg_stream_packetout(ogg_stream_state* os, ogg_packet* op)
{
    return _packetout(os, op, 1);
}
