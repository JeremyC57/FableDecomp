// CParticleAttacherDef::Copy  0x004e299b  (VC7.1 /O2 /Oy)
// void __fastcall CParticleAttacherDef::Copy(CParticleAttacherDef*, CDefClassBase const*)
// Real member function => genuine __fastcall (this in ecx, src on stack, ret 4).

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

// base class Copy at 0x431f10: __fastcall base::Copy(self, const CDefClassBase* src)
struct BaseDef {
    char base_storage[0x28];   // base class occupies [0..0x28)
};

// sub-object at +0x28 with its own Copy at 0x4e29ba: __fastcall SubDef::Copy(self, const void* src)
struct SubDef {
    void Copy_004e29ba(const void* src);
};

class CParticleAttacherDef : public BaseDef { public:
    SubDef sub;   // at offset 0x28 (BaseDef occupies [0..0x28))
    char pad[0x100];
    void Copy(const CDefClassBase* src);
};

void CParticleAttacherDef::Copy(const CDefClassBase* src)
{
    // base class copy (this in ecx, src pushed)
    ((CDefClassBase*)this)->operator=(*(const CDefClassBase*)(src));
    // sub-object copy: src advanced by 0x28
    this->sub.Copy_004e29ba((const char*)src + 0x28);
}