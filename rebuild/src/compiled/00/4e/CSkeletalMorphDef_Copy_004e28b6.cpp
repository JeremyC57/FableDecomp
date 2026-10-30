// CSkeletalMorphDef::Copy 0x004e28b6
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
struct Sub {
    void Copy(const void* src);   // 0x46078c, __fastcall(this, src)
};

class CSkeletalMorphDef { public:
    char pad[0x28];
    Sub sub;
};

void __fastcall CSkeletalMorphDef_Copy(CSkeletalMorphDef* self, void* edx, const char* src)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)(src));
    self->sub.Copy(src + 0x28);
}