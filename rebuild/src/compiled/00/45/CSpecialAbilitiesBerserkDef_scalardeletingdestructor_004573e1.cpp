#pragma optimize("s",on)
class CSpecialAbilitiesBerserkDef { public: int vt; };

extern void __fastcall CSpecialAbilitiesBerserkDef_dtor(CSpecialAbilitiesBerserkDef* def);

void* __fastcall CSpecialAbilitiesBerserkDef_scalar_deleting_destructor(CSpecialAbilitiesBerserkDef* def, int edx, unsigned int flags)
{
    CSpecialAbilitiesBerserkDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}