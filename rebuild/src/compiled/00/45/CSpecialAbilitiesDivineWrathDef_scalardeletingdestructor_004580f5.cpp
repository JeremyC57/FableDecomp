#pragma optimize("s",on)
class CSpecialAbilitiesDivineWrathDef { public: void* vftable; };
extern void __fastcall CSpecialAbilitiesDivineWrathDef_dtor(CSpecialAbilitiesDivineWrathDef* def);

void* __fastcall CSpecialAbilitiesDivineWrathDef_scalar_deleting_destructor(CSpecialAbilitiesDivineWrathDef* def, int edx_unused, unsigned int flags)
{
    CSpecialAbilitiesDivineWrathDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}