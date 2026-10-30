#pragma optimize("s",on)
// CCrateStackDef::Copy  0x004e0e98

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

// def vtable 0x0123C86C: slot 0 0x004D8A8E (implicit inline dtor); real overrides from the def vtable scan
class CCrateStackDef : public CDefClassBase { public:
    char pad[0x28 - sizeof(CDefClassBase)];
    unsigned int m28;
    unsigned int m2c;
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004DE9A1
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004E0E98
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004D50B2
};

void __fastcall Copy(CCrateStackDef* self, void* edx, const CDefClassBase* other)
{
    self->CDefClassBase::operator=(*(const CDefClassBase*)(other));
    const CCrateStackDef* o = (const CCrateStackDef*)other;
    self->m28 = o->m28;
    self->m2c = o->m2c;
}