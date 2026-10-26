#pragma optimize("s",on)
class CSpecialAbilitiesMultiArrowDef { public: int f0; };

extern void __fastcall CSpecialAbilitiesMultiArrowDef_dtor(CSpecialAbilitiesMultiArrowDef* def);

void* __fastcall CSpecialAbilitiesMultiArrowDef_scalar_deleting_destructor(CSpecialAbilitiesMultiArrowDef* def, void* edx, unsigned int flags)
{
    (void)edx;
    CSpecialAbilitiesMultiArrowDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}