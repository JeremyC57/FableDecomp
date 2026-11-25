// C3DMeshFileRootChunk::GetChunkDescription() const -- returns "3D Mesh File" as a const CCharString
// Leaf: ?GetChunkDescription@C3DMeshFileRootChunk@@UBE?BVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9C9C0, oracle length 25 bytes, thiscall `ret 4` (hidden result).
// Retail bytes:  push esi ; mov esi,[esp+8] ; push -1 ; push offset "3D Mesh File" ; mov ecx,esi ; call CCharString::CCharString(const char*,long)
//   @ 0x0099EBF0 ; mov eax,esi ; pop esi ; ret 4. The hidden result slot is constructed from the literal (length -1 = strlen); `this` is unused.
// Name: Ego_r.pdb (lib_3d_mesh_file.obj index 36, 0x00C0E0B0) and FableWin.pdb (0x033D5600) ?GetChunkDescription@C3DMeshFileRootChunk@@UBE?BVCCharString@@XZ (virtual, const, const CCharString
//   by value); the retail vftable 0x0129E090 slot 1 is this function (it overrides CChunkedFileChunk::GetChunkDescription, slot 1).
// NF422: was the free stand-in class C3DMeshFileRootChunk_Methods (?GetChunkDescription@C3DMeshFileRootChunk_Methods@@QAE?AVCCharString@@XZ); now the member on the shared
//   header, so the vftable the lib_3d_*_file unit emits (slot 1) resolves to it.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#include "engine/C3DMeshFileRootChunk.h"

const CCharString C3DMeshFileRootChunk::GetChunkDescription() const
{
    return CCharString("3D Mesh File", -1);
}
