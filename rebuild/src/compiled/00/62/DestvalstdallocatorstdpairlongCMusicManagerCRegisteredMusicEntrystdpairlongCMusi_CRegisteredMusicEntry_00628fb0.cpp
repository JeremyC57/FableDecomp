// Leaf: DestroyMembers
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
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
struct CCreatureAction_00628fb0 : CCreatureActionBase {
    CBaseIntelligentPointer Pointer;            // +0xa8
    void DestroyMembers();
};

void CCreatureAction_00628fb0::DestroyMembers()
{
    Pointer.CBaseIntelligentPointer::~CBaseIntelligentPointer();
    this->CCreatureActionBase::~CCreatureActionBase();
}
