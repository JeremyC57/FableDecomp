// CObjectFamilyDef::Copy @ 0x004611d6
class CDefClassBase;

struct SubDef {
    void Copy_004611f5(const void* src);   // member @ 0x4611f5 (thiscall)
};

struct BaseDef {
    void BaseCopy(const CDefClassBase* src);  // member @ 0x431ed0 (thiscall)
};

class CObjectFamilyDef { public:
    char pad[0x3c];
    SubDef sub;   // at +0x3c
};

void __fastcall CObjectFamilyDef_Copy(CObjectFamilyDef* self, int /*edx*/, const CDefClassBase* src)
{
    ((BaseDef*)self)->BaseCopy(src);
    ((SubDef*)((char*)self + 0x3c))->Copy_004611f5((const char*)src + 0x3c);
}