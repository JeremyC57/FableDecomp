#pragma optimize("s",on)
// CDecapitationDef::Copy @ 0x004e1225
// void __fastcall Copy(CDecapitationDef* this, CDefClassBase const* src)
struct CDefClassBase;

// Base class with the Copy @ 0x431f10 (real __fastcall member)
struct CDefBase {
    void Copy(CDefClassBase const* src); // extern, at 0x431f10
};

struct CDecapitationDef : public CDefBase {
    char pad[0x28]; // ensure f28 lands at +0x28
    unsigned int BodyParticleEffect; // +0x28
    unsigned int Head; // +0x2c
    void Copy(CDefClassBase const* src);
};

void CDecapitationDef::Copy(CDefClassBase const* src)
{
    CDefBase::Copy(src);
    this->BodyParticleEffect = ((const CDecapitationDef*)src)->BodyParticleEffect;
    this->Head = ((const CDecapitationDef*)src)->Head;
}