#pragma optimize("s",on)
class CSpecialAbilitiesSummonSpellDef { public: void* vftable; };

extern void __fastcall CSpecialAbilitiesSummonSpellDef_dtor(CSpecialAbilitiesSummonSpellDef* def);

void* __fastcall CSpecialAbilitiesSummonSpellDef_scalar_deleting_destructor(CSpecialAbilitiesSummonSpellDef* def, int /*edx*/, unsigned int flags)
{
    CSpecialAbilitiesSummonSpellDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}