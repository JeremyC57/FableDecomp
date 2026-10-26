// Leaf: DestroyMembers
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E3C6E, 19 bytes. Catalog name: std::_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>.
// Body: destroy the member at +0xc (CMap_004e34b2::~CMap_004e34b2 @ 0x004E34B2), then tail-call
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
struct CMap_004e34b2 {                      // unattributed STLport map/set: {header, count}; _M_erase @ 0x004DD782, header freed with free
    void* _M_header;
    unsigned long _M_node_count;
    ~CMap_004e34b2();                       // 0x004E34B2 (catalog label propagated from another body)
};
#pragma pack(push, 1)
struct CObject_004e3c6e : CTCBase {
    unsigned char _pad[0x8];
    CMap_004e34b2 Second;                       // +0xc
    void DestroyMembers();
};
#pragma pack(pop)

void CObject_004e3c6e::DestroyMembers()
{
    Second.~CMap_004e34b2();
    this->CTCBase::~CTCBase();
}
