// C3DMeshFileMaterialChunk::CreateMap(EMaterialMapType) -- attach and return a new MMAP material-map chunk.
// Leaf: ?CreateMap@C3DMeshFileMaterialChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileMapChunk@@@@W4EMaterialMapType@@@Z
// Retail 0x00AC0C00, 222 bytes (`ret 8`: hidden result + map type). Ego_r.pdb 0x00C3D760 / FableWin.pdb 0x0347C780
//   ?CreateMap@C3DMeshFileMaterialChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileMapChunk@@@@W4EMaterialMapType@@@Z.
// Callees: operator new 0x00BFEA1A (0x3C map chunk, 0xC count block), CChunkedFileSuperChunk::CChunkedFileSuperChunk(const char*)
//   0x00AA8360 ("MMAP" 0x0129EDD4), CCharString::CCharString() 0x0099E4B0 (x2), CChunkedFileSuperChunk::AddSubChunk 0x00AA84F0,
//   operator delete 0x00BFE9BC; the count block stores CCountedPointer<C3DMeshFileMapChunk>::DeleteData 0x00AC1460.
// Shape: the inline C3DMeshFileMapChunk(EMaterialMapType) ctor (vptr 0x0129EE34, MapType +0x2C), the (T*) counted-pointer ctor,
//   a converted CCountedPointer<CChunkedFileChunk> temporary for AddSubChunk, then the copy into the result and the local's release.
// NF192 (q4362): ported onto the shared engine/C3DMeshFileMaterialChunk.h / C3DMeshFileMapChunk.h / CChunkedFileSuperChunk.h /
//   CCountedPointer.h (was a row-local NativeChunk / NativeSuperChunk / C3DMeshFileMapChunk / CountedPointer model binding
//   ??0NativeSuperChunk@@QAE@PBD@Z at 0x00AA8360 and defining a MaterialChunkMethods::CreateMap stand-in).
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK_ADDSUBCHUNK
#define FABLE_RETAIL_C3DMESHFILEMATERIALCHUNK
#define FABLE_RETAIL_C3DMESHFILEMATERIALCHUNK_CREATEMAP
#define FABLE_RETAIL_C3DMESHFILEMAPCHUNK_TYPE_CTOR
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CChunkedFileSuperChunk.h"
#include "engine/C3DMeshFileMaterialChunk.h"
#include "engine/C3DMeshFileMapChunk.h"

CCountedPointer<C3DMeshFileMapChunk> C3DMeshFileMaterialChunk::CreateMap(EMaterialMapType type)
{
    CCountedPointer<C3DMeshFileMapChunk> map(new C3DMeshFileMapChunk(type));
    AddSubChunk(map);
    return map;
}
