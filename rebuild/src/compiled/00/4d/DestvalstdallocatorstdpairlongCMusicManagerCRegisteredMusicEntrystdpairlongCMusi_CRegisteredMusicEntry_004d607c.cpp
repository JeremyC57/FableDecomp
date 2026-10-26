// Leaf: DestroyMembers
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D607C, 19 bytes. Catalog name: std::_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>.
// Body: destroy the member at +0x10 (CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10), then tail-call
// the base CTCBase::~CTCBase @ 0x00686830. __thiscall, this = ecx.
// The +0x0 callee is a polymorphic class destructor, so this is a derived class's destructor body;
// the class itself is not attributed and is named by address.
// Engine headers for the callee classes (PDB data layouts, renamed out of the way): the local declarations
// below carry the retail identities the relocations bind.
#define CTCBase CTCBasePdb
#include "engine/CTCBase.h"
#undef CTCBase
class CTCBase { public:                            // vtable 0x0125BE0C (.?AVCTCBase@@)
    void* _vptr;
    ~CTCBase();                             // 0x00686830 (stores the vtable, tail-jumps ~CBaseClassNonCopyable)
};
struct CBaseIntelligentPointer {            // vptr + data (8 bytes); vtable 0x0129C95C
    void* _vptr;
    void* PData;
    ~CBaseIntelligentPointer();             // 0x00A01C10
};
#pragma pack(push, 1)
struct CObject_004d607c : CTCBase {
    unsigned char _pad[0xc];
    CBaseIntelligentPointer Second;             // +0x10
    void DestroyMembers();
};
#pragma pack(pop)

void CObject_004d607c::DestroyMembers()
{
    Second.~CBaseIntelligentPointer();
    this->CTCBase::~CTCBase();
}
