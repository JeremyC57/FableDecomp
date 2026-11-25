// C3DMeshFile::C3DMeshFile(const CCountedPointer<C3DMeshFileRootChunk>& root) -- chunked file over a typed root chunk
// Leaf: ??0C3DMeshFile@@AAE@ABV?$CCountedPointer@VC3DMeshFileRootChunk@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9AE60, length 96 bytes, thiscall `ret 4`.
// Callees: CChunkedFile::CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>&) @ 0x00AA7DB0, operator delete @ 0x00BFE9BC (the
//   count block). Vtable 0x0129E0D8 (rtti-vtables.tsv; slot 0 is the ??_G 0x00A9D2A0).
// The derived-typed counted pointer is converted to the base-typed one the CChunkedFile constructor takes: the converting constructor
// copies object + count block into an 8-byte temporary and bumps the count, the base constructor gets its address, and the temporary is
// released (decrement, re-read, destroy + delete on zero) before this class's vtable is stored.
// Name: Ego_r.pdb 0x00C0BF60 (lib_3d_mesh_file.obj index 10) and FableWin.pdb 0x033D5540 (lib_3d_mesh_file.obj) spell the PRIVATE
//   ??0C3DMeshFile@@AAE@ABV?$CCountedPointer@VC3DMeshFileRootChunk@@@@@Z (engine/C3DMeshFile.h FABLE_RETAIL_C3DMESHFILE_ROOT_CTOR).
//   NF419: was the public stand-in ...@@QAE@... on a row-local model; now the shared engine/C3DMeshFile.h (implicit destructor).
// Option S unit (owner ruling 2026-11-24; NF419): ONE translation unit, retail lib_3d_mesh_file.obj, for 0x00A9AE60 (this constructor,
//   the owner) and 0x00A9D2A0 (the compiler-generated ??_GC3DMeshFile@@UAEPAXI@Z, emitted with the vtable this constructor stores).
//   Ego_r.pdb lists both in that .obj (indexes 10 / 43) and so does FableWin.pdb. The thin row 0x00A9D2A0 includes this file.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILE_ROOT_CTOR
#include "engine/C3DMeshFile.h"
#include "engine/C3DMeshFileRootChunk.h"

C3DMeshFile::C3DMeshFile(const CCountedPointer<C3DMeshFileRootChunk>& root)
    : CChunkedFile(root)
{
}
