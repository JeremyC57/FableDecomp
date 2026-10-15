// SCodecInfo_00c8df70::Init() -- zero the eight-word record, then hang a calloc'd 0xE80-byte setup block off its last word
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8DF70, length 49 bytes (64 with padding).
// Leaf: Init
// The chunk label (CSoundInit::CSoundInit) is propagated; this is no constructor (`ret` with no `mov eax,esi`).
// Callees: MSVCR71 calloc (IAT 0x01440058, `call dword ptr [__imp_calloc]`, `add esp,8`).
// The shape is libvorbis's `vorbis_info_init` (the exe links "Xiph.Org libVorbis I 20030909"):
// `memset(vi, 0, sizeof(*vi)); vi->codec_setup = _ogg_calloc(1, sizeof(codec_setup_info));` -- vorbis_info is
// eight words with codec_setup last (+0x1c); the record pointer arrives in ecx (a one-argument thiscall or
// /Gr fastcall look the same). The 32-byte intrinsic memset becomes eight stores of the zero register through
// ecx, scheduled around the argument pushes; the pointer is kept in esi for the store after the call.
#include <string.h>

extern "C" __declspec(dllimport) void* __cdecl calloc(unsigned int count, unsigned int size);   // IAT 0x01440058

struct SCodecInfo_00c8df70 {                    // vorbis_info
    long  version;                              // +0x00
    long  channels;                             // +0x04
    long  rate;                                 // +0x08
    long  bitrate_upper;                        // +0x0c
    long  bitrate_nominal;                      // +0x10
    long  bitrate_lower;                        // +0x14
    long  bitrate_window;                       // +0x18
    void* codec_setup;                          // +0x1c: 0xE80-byte codec_setup_info
    void Init();
};

void SCodecInfo_00c8df70::Init()
{
    memset(this, 0, sizeof(*this));
    codec_setup = calloc(1, 0xE80);
}