// libvorbis::_vorbis_window_get(int n) -- Xiph window.c: the precomputed Vorbis power-sine window table for blocksize index n (vwin[n]: 64, 128, ... 8192 samples)
// Leaf: @_vorbis_window_get@4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C9EA70, 8 bytes, __fastcall (n in ecx), plain ret. No callees.
// Library: libpk statically links the Xiph Ogg Vorbis decoder ("Xiph.Org libVorbis I 20030909" @0x012BF240; libogg 1.1 /
//   libvorbis post-1.0.1 CVS), compiled as C with /Gr: the functions are extern "C" __fastcall.
// Identity: `mov eax,[ecx*4+0x013ABD90]`: 0x013ABD90 holds eight pointers 0x013A3E10, +0x80, +0x100, +0x200 ... +0x4000
//   apart = window.c's vwin[] over vwin64 (32 floats) .. vwin8192; its callers (0x00C900A1.., and the tail jump of the
//   landed 0x00C90C80) are block.c's window users: 0x00C90C80 is vorbis_window(v,W) (`b->window[W]-1<0 ? NULL :
//   _vorbis_window_get(b->window[W]-ci->halfrate_flag)`, backend_state +0x68, halfrate_flag +0xE78).
//   Label corrections: the catalog's "CMemoryAllocator::IsAllocatorFixedSize" is propagated, and the binding
//   ?LookupAllocatorSize@@YIKK@Z (from the landed 0x00C90C80, itself mislabelled CPoolEntryTable_00c90c80::
//   GetEntryAllocatorSize) is an invented spelling.
extern "C" float* vwin[8];              // 0x013ABD90 (static in window.c)

extern "C" float* __fastcall _vorbis_window_get(int n)
{
    return vwin[n];
}