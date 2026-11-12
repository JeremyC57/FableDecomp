// Leaf: DestroyMembers
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EE7A0, 22 bytes. Catalog name: std::_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>.
// True shape: the body is a CCreatureActionBase-derived action's destructor, not a pair destroy --
// it destroys the derived class's one CIntelligentPointer member at +0xa8
// (CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10), then tail-calls the base
// CCreatureActionBase::~CCreatureActionBase @ 0x00693EF0 (that body stores vtable 0x01259D24, whose
// RTTI is .?AVCCreatureActionBase@@). Retail CCreatureActionBase is 0xa8 bytes (PDB 0xb4, GOTCHAS
// wave 25), so the engine header's PDB layout is not used for the base. __thiscall, this = ecx.
// Name fix (agentNF23): the member's destructor binds the PDB spelling ??1CBaseIntelligentPointer@@UAE@XZ (virtual; Ego_r.pdb / FableWin.pdb) through engine/CIntelligentPointer.h, replacing the local non-virtual model (??1CBaseIntelligentPointer@@QAE@XZ); the explicit call is qualified so it stays direct.
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase

class CCreatureActionBase { public:                    // retail 0xa8 bytes
    unsigned char _base[0xa8 - 4];
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"         // CBaseIntelligentPointer: vptr + PData, virtual ~ @ 0x00A01C10
struct CCreatureAction_007ee7a0 : CCreatureActionBase {
    CBaseIntelligentPointer Pointer;            // +0xa8
    void DestroyMembers();
};

void CCreatureAction_007ee7a0::DestroyMembers()
{
    Pointer.CBaseIntelligentPointer::~CBaseIntelligentPointer();   // qualified: direct call 0x00A01C10, as retail
    this->CCreatureActionBase::~CCreatureActionBase();
}
