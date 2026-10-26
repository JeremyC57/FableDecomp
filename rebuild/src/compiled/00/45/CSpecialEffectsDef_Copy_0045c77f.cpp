// Byte-exact reconstruction of CSpecialEffectsDef::Copy @ 0x0045c77f


// Sub-object at this+0x28; its Copy is @0x45c79e (thiscall, src pushed).
struct SubEffectDef {
    void Copy(const void* src); // out-of-line -> real call
};

// Base class occupying [0,0x28); Copy @0x431f10 (thiscall, src pushed).
class CDefClassBase { public:
    CDefClassBase& operator=(const CDefClassBase& other); // out-of-line
    char pad[0x28];
};

class CSpecialEffectsDef : public CDefClassBase { public:
    SubEffectDef sub; // at +0x28
    void Copy(const CDefClassBase* src);
};

void CSpecialEffectsDef::Copy(const CDefClassBase* src)
{
    CDefClassBase::operator=(*(const CDefClassBase*)(src));
    sub.Copy((const void*)((const char*)src + 0x28));
}