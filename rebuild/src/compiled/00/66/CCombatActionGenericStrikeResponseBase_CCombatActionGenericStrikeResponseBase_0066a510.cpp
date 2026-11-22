// Leaf: DestroyMembers
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066A510, 22 bytes. Catalog name: std::_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>.
// Body: destroy the member at +0x114 (CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10), then tail-call
// the base CCombatActionBase::~CCombatActionBase @ 0x0062E900. __thiscall, this = ecx.
// The +0x0 callee is a polymorphic class destructor, so this is a derived class's destructor body;
// the class itself is not attributed and is named by address.
#define CCombatActionBase CCombatActionBasePdb
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
class CCombatActionBase { public:                  // retail 0x114 bytes (PDB 0x12c, GOTCHAS wave 25); derives from CCreatureActionBase
    unsigned char _base[0x114 - 4];
    virtual ~CCombatActionBase();                   // 0x0062E900 (tail-jumps ~CCreatureActionBase 0x00693EF0)
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(push, 1)
struct CObject_0066a510 : CCombatActionBase {
    CBaseIntelligentPointer Second;             // +0x114
    void DestroyMembers();
};
#pragma pack(pop)

void CObject_0066a510::DestroyMembers()
{
    Second.CBaseIntelligentPointer::~CBaseIntelligentPointer();
    this->CCombatActionBase::~CCombatActionBase();
}
