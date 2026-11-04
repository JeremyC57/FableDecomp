// CChunkedFileRawDataChunk::~CChunkedFileRawDataChunk() -- raw-data chunk dtor: delete[] the chunk bytes, then the inline ~CChunkedFileChunk and ~CBaseClassNonCopyable
// Leaf: ??1CChunkedFileRawDataChunk@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA7710.
// Identity: Ego_r.pdb ??1CChunkedFileRawDataChunk@@UAE@XZ; base dtor ~CChunkedFileChunk 0x00AA75C0 is empty and inlined here.
// Shape: `delete[] Data;` then the inline base destructors (overlay FABLE_RETAIL_CCHUNKEDFILERAWDATACHUNK).
void __cdecl operator delete[](void* block);                 // 0x00BFEB1C (MSVCR71 ??_V thunk)
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILERAWDATACHUNK
#include "engine/CChunkedFileRawDataChunk.h"

CChunkedFileChunk::~CChunkedFileChunk()
{
}

CChunkedFileRawDataChunk::~CChunkedFileRawDataChunk()
{
    delete[] Data;
}