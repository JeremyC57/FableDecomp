// CMeshDataBank::Close() -- empty the mesh tables (CloseMeshes), then close the underlying asynchronous bank file
// Leaf: ?Close@CMeshDataBank@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A26C50, 16 bytes: `push esi ; mov esi,ecx ; call 0x00A26900 ; mov ecx,esi ; pop esi ;
//   jmp 0x009D58A0` (the base call is a tail jump).
// Identity: Ego_r.pdb 0x00B64430 / FableWin.pdb 0x03130530 ?Close@CMeshDataBank@@UAEXXZ (virtual override of CBankFileAsync::Close).
// Callees: CMeshDataBank::CloseMeshes 0x00A26900 (Ego_r.pdb ?CloseMeshes@CMeshDataBank@@AAEXXZ, private; the spelling landed
//   0x00A27350 binds) and CBankFileAsync::Close 0x009D58A0 (?Close@CBankFileAsync@@UAEXXZ, the landed self row's leaf).
// NF176 (2026-11-20, q3886): was the free stand-in CMeshDataBank_Close(CMeshDataBank*) over a local CMeshDataBank model binding
//   ?h1@@YIXPAVCMeshDataBank@@@Z / ?h2@@YIXPAVCMeshDataBank@@@Z; now the member on engine/CMeshDataBank.h (RTTI base chain
//   CMeshDataBank : CBankFileAsync : CBankFile, opt-ins FABLE_RETAIL_CMESHDATABANK_BASES / _CTOR and the NF176 _CLOSE declarations).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CMESHDATABANK_CTOR
#define FABLE_RETAIL_CMESHDATABANK_CLOSE
#include "fable_string.h"
#include "engine/CMeshDataBank.h"

void CMeshDataBank::Close()
{
    CloseMeshes();
    CBankFileAsync::Close();
}
