// C3DMeshFileMapChunk::`scalar deleting destructor' @ 0x00AC1960 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x34 CCharString (~ @ 0x0099EAE0), +0x30 CCharString (~ @ 0x0099EAE0), then the base CChunkedFileSuperChunk (~ @ 0x00AA8490), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
// Engine headers for the callee classes (PDB data layouts, renamed out of the way): the local declarations
// below carry the retail identities the relocations bind.
#define CChunkedFileSuperChunk CChunkedFileSuperChunkPdb
#include "engine/CChunkedFileSuperChunk.h"
#undef CChunkedFileSuperChunk
#pragma optimize("t", on)
class CChunkedFileSuperChunk { public:             // vtable 0x0129E4F0 (.?AVCChunkedFileSuperChunk@@)
    virtual ~CChunkedFileSuperChunk();              // 0x00AA8490
};
class CCharString { public:                        // one word: CCharStringData*
    void* PStringData;
    ~CCharString();                         // 0x0099EAE0 (release @ 0x0099E9B0, instance count 0x013BD800)
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_00ac1960 : CChunkedFileSuperChunk {
    unsigned char _pad30[0x2c];
    CCharString Member30;                       // +0x30
    CCharString Member34;                       // +0x34
};
#pragma pack(pop)

void* __fastcall C3DMeshFileMapChunk_CompositeScalarDeletingDestructor(void* chunk, int, unsigned int flags)
{
    CObject_00ac1960* object = static_cast<CObject_00ac1960*>(chunk);
    object->Member34.~CCharString();
    object->Member30.~CCharString();
    object->CChunkedFileSuperChunk::~CChunkedFileSuperChunk();
    if (flags & 1)
        operator delete(object);
    return chunk;
}
