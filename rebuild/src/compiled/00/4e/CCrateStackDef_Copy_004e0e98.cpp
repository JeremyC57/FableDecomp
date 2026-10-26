#pragma optimize("s",on)
// CCrateStackDef::Copy  0x004e0e98

class CDefClassBase { public:
    // base class with Copy at 0x431f10 (__fastcall: ecx=this, stack=other)
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

class CCrateStackDef : public CDefClassBase { public:
    char pad[0x28 - sizeof(CDefClassBase)];
    unsigned int m28;
    unsigned int m2c;
};

void __fastcall Copy(CCrateStackDef* self, void* edx, const CDefClassBase* other)
{
    self->CDefClassBase::operator=(*(const CDefClassBase*)(other));
    const CCrateStackDef* o = (const CCrateStackDef*)other;
    self->m28 = o->m28;
    self->m2c = o->m2c;
}