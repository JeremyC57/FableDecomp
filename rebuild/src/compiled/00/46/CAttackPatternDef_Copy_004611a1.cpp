// CAttackPatternDef::Copy @ 0x004611A1
// Leaf: ?CAttackPatternDef_Copy@@YIXPAVCObjectFamilyDef@@HPBVCDefClassBase@@@Z
class CDefClassBase;

struct SubDef {
    void Copy(const void* src);   // member @ 0x4611f5 (thiscall)
};

struct BaseDef {
    void BaseCopy(const CDefClassBase* src);  // member @ 0x431ed0 (thiscall)
};

class CObjectFamilyDef { public:
    char pad[0x3c];
    SubDef sub;   // at +0x3c
};

void __fastcall CAttackPatternDef_Copy(CObjectFamilyDef* self, int /*edx*/, const CDefClassBase* src)
{
    ((BaseDef*)self)->BaseCopy(src);
    ((SubDef*)((char*)self + 0x3c))->Copy((const char*)src + 0x3c);
}
// Stand-in (owner rule 2026-11-05): CAttackPatternDef_Copy for CAttackPatternDef::Copy; RTTI vtable slot ?Copy@CAttackPatternDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
