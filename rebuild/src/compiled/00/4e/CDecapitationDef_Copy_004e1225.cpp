#pragma optimize("s",on)
// CDecapitationDef::Copy @ 0x004e1225
// void __fastcall Copy(CDecapitationDef* this, CDefClassBase const* src)

// Base class with the Copy @ 0x431f10 (real __fastcall member)
class CDefClassBase { public:
    CDefClassBase& operator=(const CDefClassBase& other); // extern, at 0x431f10
};

class CDecapitationDef : public CDefClassBase { public:
    char pad[0x28]; // ensure f28 lands at +0x28
    unsigned int BodyParticleEffect; // +0x28
    unsigned int Head; // +0x2c
    void Copy(CDefClassBase const* src);
};

void CDecapitationDef::Copy(CDefClassBase const* src)
{
    CDefClassBase::operator=(*(const CDefClassBase*)(src));
    this->BodyParticleEffect = ((const CDecapitationDef*)src)->BodyParticleEffect;
    this->Head = ((const CDecapitationDef*)src)->Head;
}