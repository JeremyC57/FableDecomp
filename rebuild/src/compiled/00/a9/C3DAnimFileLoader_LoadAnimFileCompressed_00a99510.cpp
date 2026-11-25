// Leaf: ?LoadAnimFileCompressed@C3DAnimFileLoader@@QAE?AV?$CCountedPointer@VC3DAnimFile@@@@PBXKAAVCHandleBasedMemoryPool@@@Z
// C3DAnimFileLoader::LoadAnimFileCompressed(const void* data, unsigned long size, CHandleBasedMemoryPool& pool) -- load from a (possibly LZO-compressed) memory image
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A99510, length 250 bytes (`ret 10h`: hidden result + 3 args).
// Name: Ego_r.pdb 0x00B98670 (index 23) and FableWin.pdb 0x03204220 (lib_3d_anim_file.obj).
// Callees: CMemoryDataInputStream(const void*, unsigned long) 0x00994930 / ~CDataStream 0x00993B10 (implicit stream destructors),
//   C3DAnimFileLoader::LoadAnimFile 0x00A99420 (same TU), CTemporaryBuffer 0x00A98860 / ~CTemporaryBuffer 0x00A98910, lzo1x_decompress_asm_fast 0x00C069D0,
//   operator delete 0x00BFE9BC; g_ScratchBuffer 0x013CB498; the ">>>>" tag (4-byte memcmp intrinsic).
// NF422: was a separate TU that redefined LoadAnimFile 0x00A99420 and the root-chunk constructor (strong duplicates of the unit's bodies).
// Option S unit (owner ruling 2026-11-24; NF419 b7, re-owned and widened by NF422): this function's one body lives in its retail TU-mate's
//   row 0x00A98E60 (owner of the retail lib_3d_anim_file.obj unit). The owner is included below with no code of its own; that object's
//   copy of this function grades against this address.
#include "../src/compiled/00/a9/C3DAnimFile_C3DAnimFile_00a98e60.cpp"
