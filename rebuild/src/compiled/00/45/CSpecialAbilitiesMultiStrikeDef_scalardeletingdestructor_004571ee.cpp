#pragma optimize("s",on)
struct CSpecialAbilitiesMultiStrikeDef { int f0; };
extern void __fastcall CSpecialAbilitiesMultiStrikeDef_dtor(CSpecialAbilitiesMultiStrikeDef* def);
extern void __cdecl operator_delete(void* p);

void* __fastcall CSpecialAbilitiesMultiStrikeDef_scalar_deleting_destructor(CSpecialAbilitiesMultiStrikeDef* def, int edx_ignored, unsigned int flags)
{
    (void)edx_ignored;
    CSpecialAbilitiesMultiStrikeDef_dtor(def);
    if (flags & 1)
        operator_delete(def);
    return def;
}