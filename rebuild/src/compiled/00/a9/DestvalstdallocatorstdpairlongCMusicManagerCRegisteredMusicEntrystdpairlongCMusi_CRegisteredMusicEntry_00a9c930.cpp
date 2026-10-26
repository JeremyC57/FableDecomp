// Leaf: DestroyMembers
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A9C930, 19 bytes. Catalog name: std::_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>.
// Body: destroy the member at +0x4c (CCharString::~CCharString @ 0x0099EAE0), then tail-call
// the base CChunkedFileSuperChunk::~CChunkedFileSuperChunk @ 0x00AA8490. __thiscall, this = ecx.
// The +0x0 callee is a polymorphic class destructor, so this is a derived class's destructor body;
// the class itself is not attributed and is named by address.
// Engine headers for the callee classes (PDB data layouts, renamed out of the way): the local declarations
// below carry the retail identities the relocations bind.
#define CChunkedFileSuperChunk CChunkedFileSuperChunkPdb
#include "engine/CChunkedFileSuperChunk.h"
#undef CChunkedFileSuperChunk
class CChunkedFileSuperChunk { public:             // vtable 0x0129E4F0 (.?AVCChunkedFileSuperChunk@@)
    virtual ~CChunkedFileSuperChunk();              // 0x00AA8490
};
class CCharString { public:                        // one word: CCharStringData*
    void* PStringData;
    ~CCharString();                         // 0x0099EAE0 (release @ 0x0099E9B0, instance count 0x013BD800)
};
#pragma pack(push, 1)
struct CObject_00a9c930 : CChunkedFileSuperChunk {
    unsigned char _pad[0x48];
    CCharString Second;                         // +0x4c
    void DestroyMembers();
};
#pragma pack(pop)

void CObject_00a9c930::DestroyMembers()
{
    Second.~CCharString();
    this->CChunkedFileSuperChunk::~CChunkedFileSuperChunk();
}
