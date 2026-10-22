#pragma optimize("s",on)
struct CSpecialAbilitiesDrainLifeDef;

extern void __fastcall CSpecialAbilitiesDrainLifeDef_dtor(CSpecialAbilitiesDrainLifeDef* def);
extern void __cdecl fable_operator_delete(void* p);

void* __fastcall CSpecialAbilitiesDrainLifeDef_scalar_deleting_destructor(CSpecialAbilitiesDrainLifeDef* def, int edx, unsigned int flags)
{
    (void)edx;
    CSpecialAbilitiesDrainLifeDef_dtor(def);
    if (flags & 1)
        fable_operator_delete(def);
    return def;
}