// Leaf: DestroyMembers
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E7FA0, 22 bytes. Catalog name: std::_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>.
// True shape: the body is a CCreatureActionBase-derived action's destructor, not a pair destroy --
// it destroys the derived class's one CWideString member at +0xa8
// (CWideString::~CWideString @ 0x0099B510: the wide-string release, NoRefs at data+0xc, free(), instance count 0x013BCA20), then tail-calls the base
// CCreatureActionBase::~CCreatureActionBase @ 0x00693EF0 (that body stores vtable 0x01259D24, whose
// RTTI is .?AVCCreatureActionBase@@). Retail CCreatureActionBase is 0xa8 bytes (PDB 0xb4, GOTCHAS
// wave 25), so the engine header's PDB layout is not used for the base. __thiscall, this = ecx.
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase

struct CCreatureActionBase {                    // retail 0xa8 bytes
    unsigned char _base[0xa8];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CWideString {                            // one word: CWideStringData*
    void* PStringData;
    ~CWideString();                             // 0x0099B510
};
struct CCreatureAction_007e7fa0 : CCreatureActionBase {
    CWideString Text;                           // +0xa8
    void DestroyMembers();
};

void CCreatureAction_007e7fa0::DestroyMembers()
{
    Text.~CWideString();
    this->CCreatureActionBase::~CCreatureActionBase();
}
