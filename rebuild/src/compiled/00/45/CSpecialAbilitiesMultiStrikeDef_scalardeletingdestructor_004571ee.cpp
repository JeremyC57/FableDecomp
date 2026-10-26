#pragma optimize("s",on)
class CSpecialAbilitiesMultiStrikeDef { public: int f0; };
extern void __fastcall CSpecialAbilitiesMultiStrikeDef_dtor(CSpecialAbilitiesMultiStrikeDef* def);

void* __fastcall CSpecialAbilitiesMultiStrikeDef_scalar_deleting_destructor(CSpecialAbilitiesMultiStrikeDef* def, int edx_ignored, unsigned int flags)
{
    (void)edx_ignored;
    CSpecialAbilitiesMultiStrikeDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}