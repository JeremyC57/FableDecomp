// Leaf: ??1CChunkedFile@@UAE@XZ
// CChunkedFile::~CChunkedFile() -- own vtable, destroy FileComment, release the root chunk's count block (destroy + free on the last
//   reference, re-reading the block through `this` after the decrement), null the pointer pair and tail-jump into the base destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA7DF0, length 74 bytes, thiscall, ends `jmp 0x0099A300`.
// Callees: CCharString::~CCharString 0x0099EAE0 (FileComment +0x10), the count block's DeleteFunc (+4) on its Data (+8), operator
//   delete 0x00BFE9BC (the block), ~CBaseClassNonCopyable 0x0099A300 (jmp; table ??1CBaseClassNonCopyable@@MAE@XZ).
// Name (NF416, T3 nf401 0x00AA9190 identity): ??1CChunkedFile@@UAE@XZ.
//   (1) FableWin.pdb names it in lib_chunked_file.obj (0x03469490).
//   (2) Ego_r.pdb names it in the same .obj (index 31, 0x00C1C4F0, 76 bytes), right after the ctor (index 30, 0x00C1C4B0), as
//       retail 0x00AA7DF0 follows the ctor 0x00AA7DB0.
//   Retail: a raw scan of CChunkedFile's vftable 0x0129E4E8 as an immediate finds three stores: the ctor 0x00AA7DB0, this body,
//   and the ??_G 0x00AA9190. This is the only non-deleting one. The C3DMeshFile and C3DAnimFile deleting destructors call it
//   directly (0x00A9D2A3 in 0x00A9D2A0, 0x00A9A733 in 0x00A9A730). Virtual: vftable slot 0 is the ??_G 0x00AA9190.
//   Before NF416 the row strongly defined ??1CChunkedFile@@QAE@XZ (non-virtual, a row-local CBase model). The real spelling was
//   then strongly defined by 0x00AA9190's object.
// Option S unit (owner ruling 2026-11-24; NF416): this function's one body lives in its retail TU-mate's row 0x00AA7DB0 (the ctor;
//   lib_chunked_file.obj in both PDBs), included below with no code of its own, as the EH-funclet rows include their parent. That
//   object's ~CChunkedFile grades against this address.
#include "../src/compiled/00/aa/CChunkedFile_CChunkedFile_00aa7db0.cpp"
