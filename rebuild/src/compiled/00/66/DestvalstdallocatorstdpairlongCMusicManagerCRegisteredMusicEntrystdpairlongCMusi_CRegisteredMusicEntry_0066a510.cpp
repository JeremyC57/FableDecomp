// Leaf: DestroyMembers
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066A510, 22 bytes. Catalog name: std::_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>.
// Body: destroy the member at +0x114 (CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10), then tail-call
// the base CCombatActionBase::~CCombatActionBase @ 0x0062E900. __thiscall, this = ecx.
// The +0x0 callee is a polymorphic class destructor, so this is a derived class's destructor body;
// the class itself is not attributed and is named by address.
#define CCombatActionBase CCombatActionBasePdb
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
struct CCombatActionBase {                  // retail 0x114 bytes (PDB 0x12c, GOTCHAS wave 25); derives from CCreatureActionBase
    unsigned char _base[0x114];
    ~CCombatActionBase();                   // 0x0062E900 (tail-jumps ~CCreatureActionBase 0x00693EF0)
};
struct CBaseIntelligentPointer {            // vptr + data (8 bytes); vtable 0x0129C95C
    void* _vptr;
    void* PData;
    ~CBaseIntelligentPointer();             // 0x00A01C10
};
#pragma pack(push, 1)
struct CObject_0066a510 : CCombatActionBase {
    CBaseIntelligentPointer Second;             // +0x114
    void DestroyMembers();
};
#pragma pack(pop)

void CObject_0066a510::DestroyMembers()
{
    Second.~CBaseIntelligentPointer();
    this->CCombatActionBase::~CCombatActionBase();
}
