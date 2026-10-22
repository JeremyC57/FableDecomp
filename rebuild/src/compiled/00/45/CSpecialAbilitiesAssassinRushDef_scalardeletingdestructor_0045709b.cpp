#pragma optimize("s",on)
struct CSpecialAbilitiesAssassinRushDef;
extern void __fastcall CLBP_dtor(CSpecialAbilitiesAssassinRushDef* def);
extern void __cdecl CLBP_operator_delete(void* def);

void* __fastcall CSpecialAbilitiesAssassinRushDef_scalar_deleting_destructor(CSpecialAbilitiesAssassinRushDef* def, int edx_dummy, unsigned int flags)
{
    (void)edx_dummy;
    CLBP_dtor(def);
    if (flags & 1) {
        CLBP_operator_delete(def);
    }
    return def;
}