#pragma optimize("s",on)
class CSpecialAbilitiesDrainLifeDef;

extern void __fastcall CSpecialAbilitiesDrainLifeDef_dtor(CSpecialAbilitiesDrainLifeDef* def);

void* __fastcall CSpecialAbilitiesDrainLifeDef_scalar_deleting_destructor(CSpecialAbilitiesDrainLifeDef* def, int edx, unsigned int flags)
{
    (void)edx;
    CSpecialAbilitiesDrainLifeDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}