// CChunkedFileRawDataChunk::CChunkedFileRawDataChunk(const char* type) -- the inlined CChunkedFileChunk
// shell (CBase, base vtable, no parent, zeroed + copied chunk type) then the derived vtable 0x0129E4B8
// (RTTI: CChunkedFileRawDataChunk) and no data (Data = 0, ByteSize = 0).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA76D0, length 59 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0. Manifest label (CChunkedFileSuperChunk ctor) corrected from the vtable.
// CBase: the engine's polymorphic root (vtable @ 0x0129A7C4 in the ctor, 0x01231710 in the dtor);
// both members are out of line in retail.
class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable();                                    // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                                   // 0x0099A300
};
#include <string.h>
#include "engine/CChunkedFileChunk.h"      // PDB layout (ChunkType[5] +4, Parent +0xc) agrees with the bytes; the
                                          // header is a flat POD, so the constructor needs this polymorphic twin
class CChunkedFileSuperChunk;
struct CChunkedFileChunk_Retail : CBaseClassNonCopyable {
    char                    ChunkType[5];       // +0x04
    CChunkedFileSuperChunk* Parent;             // +0x0c
    CChunkedFileChunk_Retail(const char* type)
        : Parent(0)
    {
        memset(ChunkType, 0, sizeof(ChunkType));
        memcpy(ChunkType, type, 4);
    }
};
FABLE_STATIC_ASSERT(sizeof(CChunkedFileChunk_Retail) == sizeof(CChunkedFileChunk));
#include "engine/CChunkedFileRawDataChunk.h"   // PDB: Data +0x10, ByteSize +0x14 agree with the bytes
struct CChunkedFileRawDataChunk_Retail : CChunkedFileChunk_Retail {
    void*         Data;                         // +0x10 (uchar*)
    unsigned long ByteSize;                     // +0x14
    CChunkedFileRawDataChunk_Retail(const char* type);
    virtual CChunkedFileChunk_Retail* CreateNewInstance();
};
FABLE_STATIC_ASSERT(sizeof(CChunkedFileRawDataChunk_Retail) == sizeof(CChunkedFileRawDataChunk));

CChunkedFileRawDataChunk_Retail::CChunkedFileRawDataChunk_Retail(const char* type)
    : CChunkedFileChunk_Retail(type), Data(0), ByteSize(0)
{
}