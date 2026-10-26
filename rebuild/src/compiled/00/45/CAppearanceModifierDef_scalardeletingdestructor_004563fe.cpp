#pragma optimize("s",on)
class CAppearanceModifierDef { public:
    int f0;
    int Parent;
    void* f8;
    void CAppearanceModifierDef_dtor();
    void* CAppearanceModifierDef_scalar_deleting_destructor(unsigned int flags);
};

void* CAppearanceModifierDef::CAppearanceModifierDef_scalar_deleting_destructor(unsigned int flags)
{
    this->CAppearanceModifierDef_dtor();
    if (flags & 1) {
        operator delete(this);
    }
    return this;
}