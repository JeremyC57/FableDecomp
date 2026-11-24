// Leaf: ?Copy@CObjectFamilyDef@@UAEXPBVCDefClassBase@@@Z
// CObjectFamilyDef::Copy @ 0x004611d6
class CDefClassBase;

struct SubDef {
    void Copy_004611f5(const void* src);   // member @ 0x4611f5 (thiscall)
};

struct BaseDef {
    void BaseCopy(const CDefClassBase* src);  // member @ 0x431ed0 (thiscall)
};

class CObjectFamilyDef { public:
    char pad[0x38];
    SubDef sub;   // at +0x3c
    virtual void Copy(const class CDefClassBase*);
};

void CObjectFamilyDef::Copy(const class CDefClassBase* src)
{
    CObjectFamilyDef* self = this;
    ((BaseDef*)self)->BaseCopy(src);
    ((SubDef*)((char*)self + 0x3c))->Copy_004611f5((const char*)src + 0x3c);
}
// Flatten route (flatten_pilot v4, NF324): CObjectFamilyDef_Copy -> CObjectFamilyDef::Copy, the RTTI-placed PDB member; class model: the row's local class model.
