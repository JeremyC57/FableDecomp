// CChunkedFileChunk::CChunkedFileChunk(const char* type) -- CBase shell with no parent and the four-byte
// chunk type copied into the zeroed five-byte ChunkType buffer. Vtable 0x0129E474 (RTTI: CChunkedFileChunk).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA7590, length 47 bytes.
// Callees: CBase::CBase @ 0x0099A2F0. memset(5)/memcpy(4) are the intrinsics (dword + byte zero, dword copy).
// CBase: the engine's polymorphic root (vtable @ 0x0129A7C4 in the ctor, 0x01231710 in the dtor);
// both members are out of line in retail.
struct CBase {
    virtual void VSlot0();
    CBase();                                    // 0x0099A2F0
    ~CBase();                                   // 0x0099A300
};
#include <string.h>
#include "engine/CChunkedFileChunk.h"      // PDB layout (ChunkType[5] +4, Parent +0xc) agrees with the bytes; the
                                          // header is a flat POD, so the constructor needs this polymorphic twin
struct CChunkedFileSuperChunk;
struct CChunkedFileChunk_Retail : CBase {
    char                    ChunkType[5];       // +0x04
    CChunkedFileSuperChunk* Parent;             // +0x0c
    CChunkedFileChunk_Retail(const char* type);
};
FABLE_STATIC_ASSERT(sizeof(CChunkedFileChunk_Retail) == sizeof(CChunkedFileChunk));

CChunkedFileChunk_Retail::CChunkedFileChunk_Retail(const char* type)
    : Parent(0)
{
    memset(ChunkType, 0, sizeof(ChunkType));
    memcpy(ChunkType, type, 4);
}