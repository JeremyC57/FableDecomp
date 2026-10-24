// CChunkedFileRawDataChunk::SetData(const void* data, unsigned long size) -- replace the raw chunk's byte buffer with a fresh copy of `size` bytes from `data`
// Leaf: ?SetData@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA7760, 64 bytes, thiscall `ret 8`.
// Identity: Ego_r.pdb `?SetData@CChunkedFileRawDataChunk@@QAEXPBXK@Z` (the method between IsUnknownType 0x00AA7740 and
//   ReadFromFile 0x00AA77A0, the same order as the donor's 0x00C1BF20/0x00C1BF40/0x00C1BF80); no direct caller in retail
//   (an unreferenced out-of-line copy). The catalog's "Buffer_Reallocate_And_Copy" is descriptive.
// Callees: operator delete[] 0x00BFEB1C, operator new[] 0x00BFEB22 (MSVCR71 import thunks, one shared `add esp,8`).
// Shape: the twin of the landed ReadFromFile 0x00AA77A0: free the old buffer, allocate the new one, store ByteSize and Data,
//   then the inline memcpy intrinsic (`shr ecx,2 ; rep movsd ; and ecx,3 ; rep movsb`). `this` uses
//   engine/CChunkedFileRawDataChunk.h (Data +0x10, ByteSize +0x14), with the method declared on a methods-only
//   derivation as in the landed siblings.
#include "engine/CChunkedFileRawDataChunk.h"
void __cdecl operator delete[](void* block);       // MSVCR71 import thunk 0x00BFEB1C (the lane <new> lacks it)
void* __cdecl operator new[](unsigned int size);   // MSVCR71 import thunk 0x00BFEB22 (the lane <new> lacks it)

extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CChunkedFileRawDataChunk_Methods : CChunkedFileRawDataChunk {
    void SetData(const void* data, unsigned long size);
};

void CChunkedFileRawDataChunk_Methods::SetData(const void* data, unsigned long size)
{
    delete[] (unsigned char*)Data;
    Data = new unsigned char[size];
    ByteSize = size;
    memcpy(Data, data, size);
}