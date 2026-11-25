// Leaf: ??_GCChunkedFile@@UAEPAXI@Z
// CChunkedFile::`scalar deleting destructor'(unsigned int flags) -- ~CChunkedFile expanded (vtable store, ~CCharString on FileComment,
//   the root chunk's count block released, the pointer pair nulled, ~CBaseClassNonCopyable called), then operator delete when flags & 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA9190, length 95 bytes, thiscall, `ret 4`. Vftable 0x0129E4E8 slot 0 (rtti-vtables.tsv).
// Callees: CCharString::~CCharString 0x0099EAE0, the block's DeleteFunc (`call [eax+4]`), operator delete 0x00BFE9BC (the block, and
//   `this` when flags & 1), ~CBaseClassNonCopyable 0x0099A300.
// Option S unit (owner ruling 2026-11-24; NF416): this function's one body lives in its retail TU-mate's row 0x00AA7DB0. FableWin.pdb
//   and Ego_r.pdb both list ??_GCChunkedFile@@UAEPAXI@Z, the ctor and ??1CChunkedFile@@UAE@XZ in lib_chunked_file.obj (Ego_r indexes
//   63 / 30 / 31). The owner is included below with no code of its own, as the EH-funclet rows include their parent; that object's
//   compiler-generated ??_G copy grades against this address. Before the unit this row strongly defined its own ??1CChunkedFile@@UAE@XZ
//   on a row-local CBaseClass model. Now that 0x00AA7DF0 carries that PDB spelling, the row's copy would be a duplicate.
#include "../src/compiled/00/aa/CChunkedFile_CChunkedFile_00aa7db0.cpp"
