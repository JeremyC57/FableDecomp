// Leaf: DestroyMembers
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00628FB0, 22 bytes. Catalog name: std::_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>.
// True shape: the body is a CCreatureActionBase-derived action's destructor, not a pair destroy --
// it destroys the derived class's one CIntelligentPointer member at +0xa8
// (CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10), then tail-calls the base
// CCreatureActionBase::~CCreatureActionBase @ 0x00693EF0 (that body stores vtable 0x01259D24, whose
// RTTI is .?AVCCreatureActionBase@@). Retail CCreatureActionBase is 0xa8 bytes (PDB 0xb4, GOTCHAS
// wave 25), so the engine header's PDB layout is not used for the base. __thiscall, this = ecx.
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase

class CCreatureActionBase { public:                    // retail 0xa8 bytes
    unsigned char _base[0xa8 - 4];
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CCreatureAction_00628fb0 : CCreatureActionBase {
    CBaseIntelligentPointer Pointer;            // +0xa8
    void DestroyMembers();
};

void CCreatureAction_00628fb0::DestroyMembers()
{
    Pointer.CBaseIntelligentPointer::~CBaseIntelligentPointer();
    this->CCreatureActionBase::~CCreatureActionBase();
}
