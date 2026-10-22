#pragma optimize("s",on)
struct CSpecialAbilitiesMultiArrowDef { int f0; };

extern void __fastcall CSpecialAbilitiesMultiArrowDef_dtor(CSpecialAbilitiesMultiArrowDef* def);
extern void __cdecl eng_operator_delete(void* p);

void* __fastcall CSpecialAbilitiesMultiArrowDef_scalar_deleting_destructor(CSpecialAbilitiesMultiArrowDef* def, void* edx, unsigned int flags)
{
    (void)edx;
    CSpecialAbilitiesMultiArrowDef_dtor(def);
    if (flags & 1)
        eng_operator_delete(def);
    return def;
}