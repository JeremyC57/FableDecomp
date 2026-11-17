// CAttackPatternDef::Copy @ 0x004611A1
// Leaf: ?CAttackPatternDef_Copy@@YIXPAVCObjectFamilyDef@@HPBVCDefClassBase@@@Z
// NF48 (round 7): the +0x3c member is Ego_r.pdb CAttackPatternDef::AttackPattern, a STLport 4.6.2 std::vector<long> from the shared
// stlport/fable_containers.h (was the stand-in SubDef::Copy); FABLE_RETAIL_STLP_VECTOR_ASSIGN declares vector<long>::operator=
// (0x00454886) out of line.
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "stlport/fable_containers.h"
class CDefClassBase;

struct BaseDef {
    void BaseCopy(const CDefClassBase* src);  // member @ 0x431ed0 (thiscall)
};

class CObjectFamilyDef { public:
    char pad[0x3c];
    std::vector<long> AttackPattern;   // +0x3c (Ego_r.pdb CAttackPatternDef::AttackPattern)
};

void __fastcall CAttackPatternDef_Copy(CObjectFamilyDef* self, int /*edx*/, const CDefClassBase* src)
{
    ((BaseDef*)self)->BaseCopy(src);
    self->AttackPattern = ((const CObjectFamilyDef*)src)->AttackPattern;
}
// Stand-in (owner rule 2026-11-05): CAttackPatternDef_Copy for CAttackPatternDef::Copy; RTTI vtable slot ?Copy@CAttackPatternDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
