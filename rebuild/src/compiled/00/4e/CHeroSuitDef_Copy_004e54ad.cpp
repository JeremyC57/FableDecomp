// CHeroSuitDef::Copy @ 0x004e54ad
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
struct SubDef {
    void Copy(SubDef const* src);          // @ 0x4e54cc, this in ecx, param on stack
};
class CHeroSuitDef { public:
    char basepad[0x28];
    SubDef sub;   // at +0x28
};

void __fastcall CHeroSuitDef_Copy(CHeroSuitDef* self, void* edx, CDefClassBase const* src)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)(src));
    self->sub.Copy((SubDef const*)((char const*)src + 0x28));
}