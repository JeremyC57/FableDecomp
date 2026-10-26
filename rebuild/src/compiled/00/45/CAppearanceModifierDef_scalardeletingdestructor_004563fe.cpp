#pragma optimize("s",on)
class CAppearanceModifierDef { public:
    int f0;
    int Parent;
    void* f8;
    void CAppearanceModifierDef_dtor();
    void* CAppearanceModifierDef_scalar_deleting_destructor(unsigned int flags);
};
extern void __cdecl eng_operator_delete(void* p);

void* CAppearanceModifierDef::CAppearanceModifierDef_scalar_deleting_destructor(unsigned int flags)
{
    this->CAppearanceModifierDef_dtor();
    if (flags & 1) {
        eng_operator_delete(this);
    }
    return this;
}