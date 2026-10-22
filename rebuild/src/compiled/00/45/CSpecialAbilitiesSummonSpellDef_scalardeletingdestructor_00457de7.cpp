#pragma optimize("s",on)
struct CSpecialAbilitiesSummonSpellDef { void* vftable; };

extern void __fastcall CSpecialAbilitiesSummonSpellDef_dtor(CSpecialAbilitiesSummonSpellDef* def);
extern void __cdecl op_delete(void* p);

void* __fastcall CSpecialAbilitiesSummonSpellDef_scalar_deleting_destructor(CSpecialAbilitiesSummonSpellDef* def, int /*edx*/, unsigned int flags)
{
    CSpecialAbilitiesSummonSpellDef_dtor(def);
    if (flags & 1)
        op_delete(def);
    return def;
}