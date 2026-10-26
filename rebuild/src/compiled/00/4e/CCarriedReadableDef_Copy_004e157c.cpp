#pragma optimize("s",on)
// CCarriedReadableDef::Copy  0x004e157c
#include "engine/CCarriedReadableDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CDefClassBase { public: int dummy; 
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};


// base Copy(other) — stdcall callee taking other on the stack (cleans its own arg)

void __fastcall CCarriedReadableDef_Copy(CCarriedReadableDef* self, void* /*edx*/, CDefClassBase const* other)
{
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(other));
    const CCarriedReadableDef* o = (const CCarriedReadableDef*)other;
    self->TitleTextTag_TablePos = o->TitleTextTag_TablePos;
    self->BodyTextTag_TablePos = o->BodyTextTag_TablePos;
}