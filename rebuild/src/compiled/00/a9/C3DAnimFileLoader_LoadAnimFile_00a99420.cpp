// Leaf: ?LoadAnimFile@C3DAnimFileLoader@@QAE?AV?$CCountedPointer@VC3DAnimFile@@@@AAVCDataInputStream@@@Z
// C3DAnimFileLoader::LoadAnimFile(CDataInputStream&) -- new root chunk from the registered prototype, a new C3DAnimFile around it, load the chunks, return the file
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A99420, length 240 bytes (`ret 8`: hidden result + stream).
// Name: Ego_r.pdb 0x00B98590 (index 22) and FableWin.pdb 0x032040F0 (lib_3d_anim_file.obj).
// Callees: the root prototype's CreateNewInstance (slot 9, +0x24), operator new 0x00BFEA1A, the private C3DAnimFile constructor 0x00A98F40 (same TU:
//   its body is visible, so the root pointer's count block stays in ebp across the call), the counted pointers' __fastcall DeleteData
//   0x00A99FE0 / 0x00A9A000, ?LoadChunkedFile@CChunkedFileLoader@@IAEXAAVCChunkedFile@@AAVCDataInputStream@@@Z 0x00AA79C0, operator delete 0x00BFE9BC.
// NF422: was a separate TU defining the root-chunk constructor under the public stand-in ...@@QAE@... (a strong duplicate of the unit's
//   private one) on row-local class models whose pick-any ??_GC3DAnimFile called a 5-byte ??1C3DAnimFile thunk retail lacks.
// Option S unit (owner ruling 2026-11-24; NF419 b7, re-owned and widened by NF422): this function's one body lives in its retail TU-mate's
//   row 0x00A98E60 (owner of the retail lib_3d_anim_file.obj unit). The owner is included below with no code of its own; that object's
//   copy of this function grades against this address.
#include "../src/compiled/00/a9/C3DAnimFile_C3DAnimFile_00a98e60.cpp"
