#pragma optimize("s",on)
class CSpecialAbilitiesAssassinRushDef;
extern void __fastcall CLBP_dtor(CSpecialAbilitiesAssassinRushDef* def);

void* __fastcall CSpecialAbilitiesAssassinRushDef_scalar_deleting_destructor(CSpecialAbilitiesAssassinRushDef* def, int edx_dummy, unsigned int flags)
{
    (void)edx_dummy;
    CLBP_dtor(def);
    if (flags & 1) {
        operator delete(def);
    }
    return def;
}