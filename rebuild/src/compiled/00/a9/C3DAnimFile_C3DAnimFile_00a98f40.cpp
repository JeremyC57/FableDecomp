// C3DAnimFile::C3DAnimFile(const CCountedPointer<C3DAnimFileRootChunk>& root) -- chunked file over a typed root chunk
// Leaf: ??0C3DAnimFile@@AAE@ABV?$CCountedPointer@VC3DAnimFileRootChunk@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A98F40, length 96 bytes, thiscall `ret 4`.
// Callees: CChunkedFile::CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>&) @ 0x00AA7DB0, operator delete @ 0x00BFE9BC (the
//   count block). Vtable 0x0129E034 (rtti-vtables.tsv; one slot, the ??_G 0x00A9A730).
// The derived-typed counted pointer is converted to the base-typed one the CChunkedFile constructor takes: the converting constructor
// copies object + count block into an 8-byte temporary and bumps the count, the base constructor gets its address, and the temporary is
// released (decrement, re-read, destroy + delete on zero) before this class's vtable is stored.
// Name: Ego_r.pdb 0x00B980A0 (lib_3d_anim_file.obj index 14) and FableWin.pdb 0x03204CD0 (lib_3d_anim_file.obj) spell the PRIVATE
//   ??0C3DAnimFile@@AAE@ABV?$CCountedPointer@VC3DAnimFileRootChunk@@@@@Z (engine/C3DAnimFile.h declares it private). NF419: was the
//   public stand-in ...@@QAE@... on a row-local model; now the shared engine/C3DAnimFile.h with its implicit destructor.
// Option S unit (owner ruling 2026-11-24; NF419): ONE translation unit, retail lib_3d_anim_file.obj, for 0x00A98F40 (this constructor,
//   the owner) and 0x00A9A730 (the compiler-generated ??_GC3DAnimFile@@UAEPAXI@Z, emitted with the vtable this constructor stores).
//   Ego_r.pdb lists both in that .obj (indexes 14 / 36) and so does FableWin.pdb. The thin row 0x00A9A730 includes this file.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DANIMFILE_IMPLICIT_DESTRUCTOR
#include "engine/C3DAnimFile.h"
#include "engine/C3DAnimFileRootChunk.h"

C3DAnimFile::C3DAnimFile(const CCountedPointer<C3DAnimFileRootChunk>& root)
    : CChunkedFile(root)
{
}
