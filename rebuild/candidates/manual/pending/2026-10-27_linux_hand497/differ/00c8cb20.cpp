// libvorbis::ov_test_callbacks(void* f, OggVorbis_File* vf, char* initial, long ibytes) -- Xiph vorbisfile.c (libpk fork without ov_callbacks): the partial open, _ov_open1 only
// Grade: DIFFER(5v22) -- retail calls the static _ov_open1 0x00C8C9A0 with LTCG registers (initial in eax, vf in ebx, f and ibytes pushed, ebx saved); any C declaration gives a 5-byte tail jmp (lane gap: LTCG)
// Leaf: @ov_test_callbacks@16
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8CB20, 22 bytes, __fastcall (f ecx, vf edx, initial/ibytes on the stack),
//   ret 8. Callee: _ov_open1 0x00C8C9A0 (static; LTCG: f and ibytes on the stack, initial in eax, vf in ebx).
// Library: libpk statically links the Xiph Ogg Vorbis decoder ("Xiph.Org libVorbis I 20030909" @0x012BF240; libogg 1.1 /
//   libvorbis post-1.0.1 CVS), compiled as C with /Gr: the functions are extern "C" __fastcall.
// Identity: vorbisfile.c's ov_open_callbacks 0x00C8CAE0 (_ov_open1 then _ov_open2 0x00C8CA90), ov_open 0x00C8CB10 (`jmp
//   ov_open_callbacks`), then this (_ov_open1 alone) and ov_test 0x00C8CB40 (`jmp` here) -- the Xiph source order
//   ov_open_callbacks, ov_open, ov_test_callbacks, ov_test. The catalog label (gap-scan) is a placeholder.
typedef struct OggVorbis_File OggVorbis_File;
extern "C" int __fastcall _ov_open1(void* f, OggVorbis_File* vf, char* initial, long ibytes);   // 0x00C8C9A0 (static)

extern "C" int __fastcall ov_test_callbacks(void* f, OggVorbis_File* vf, char* initial, long ibytes)
{
    return _ov_open1(f, vf, initial, ibytes);
}
