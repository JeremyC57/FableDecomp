// CIdleSchedulerDef::Copy(const CDefClassBase* other) -- slot 19 of the idle-scheduler def: `*this = *static_cast<const CIdleSchedulerDef*>(other)`
//   through the implicit assignment, expanded in place: CParentDefClassBase::operator= 0x00431ED0, then the Period
//   map<EIdleStateGroup,long> +0x3c through its out-of-line STLport _Rb_tree::operator= 0x004E610D (map's own operator= is implicit and inline)
// Leaf: ?Copy@CIdleSchedulerDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E60EE, 31 bytes, thiscall `ret 4`, no return value.
// Identity: Ego_r.pdb 0x00507B10 / FableWin.pdb 0x01BBBB79 ?Copy@CIdleSchedulerDef@@UAEXPBVCDefClassBase@@@Z. Layout: Ego_r.pdb
//   CIdleSchedulerDef (80 bytes) = CParentDefClassBase + Period (map<EIdleStateGroup,long>, PDB +0x40 -> retail +0x3c: the retail base is
//   0x39 bytes and the STLport map 12). No shared header models CIdleSchedulerDef, so it is declared here; EIdleStateGroup is the enum
//   engine/CTCIdleScheduler.h forward-declares.
// NF70 (round 7, land1117fu q10783): was a local stand-in (CDefClassBase::Copy_00431ed0 / SubMember::Copy_004e610d); now the PDB class
//   and member over the shared CParentDefClassBase.h and stlport/fable_map.h.
#pragma pack(push, 1)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)
#include "stlport/fable_map.h"
enum EIdleStateGroup;

class CIdleSchedulerDef : public CParentDefClassBase {
public:
    std::map<EIdleStateGroup, long> Period;            // +0x3c
    virtual void Copy(const CDefClassBase* other);     // slot 19
};

void CIdleSchedulerDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CIdleSchedulerDef*>(other);
}
