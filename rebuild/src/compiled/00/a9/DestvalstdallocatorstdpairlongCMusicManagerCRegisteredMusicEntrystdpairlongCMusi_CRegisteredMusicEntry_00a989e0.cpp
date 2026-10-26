// Leaf: DestroyMembers
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A989E0, 19 bytes. Catalog name: std::_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>.
// Body: destroy the member at +0x10 (CCharString::~CCharString @ 0x0099EAE0), then tail-call
// the base CChunkedFileChunk::~CChunkedFileChunk @ 0x00AA75C0. __thiscall, this = ecx.
// The +0x0 callee is a polymorphic class destructor, so this is a derived class's destructor body;
// the class itself is not attributed and is named by address.
// Engine headers for the callee classes (PDB data layouts, renamed out of the way): the local declarations
// below carry the retail identities the relocations bind.
#define CChunkedFileChunk CChunkedFileChunkPdb
#include "engine/CChunkedFileChunk.h"
#undef CChunkedFileChunk
class CChunkedFileChunk { public:                  // vtable 0x0129E474 (.?AVCChunkedFileChunk@@)
    virtual ~CChunkedFileChunk();                   // 0x00AA75C0 (stores the vtable, tail-jumps ~CBaseClassNonCopyable)
};
class CCharString { public:                        // one word: CCharStringData*
    void* PStringData;
    ~CCharString();                         // 0x0099EAE0 (release @ 0x0099E9B0, instance count 0x013BD800)
};
#pragma pack(push, 1)
struct CObject_00a989e0 : CChunkedFileChunk {
    unsigned char _pad[0xc];
    CCharString Second;                         // +0x10
    void DestroyMembers();
};
#pragma pack(pop)

void CObject_00a989e0::DestroyMembers()
{
    Second.~CCharString();
    this->CChunkedFileChunk::~CChunkedFileChunk();
}
