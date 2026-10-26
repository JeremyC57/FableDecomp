#pragma optimize("s",on)
class CSpecialAbilitiesForcePushDef { public:
    int f0;
};

extern void __fastcall CSpecialAbilitiesForcePushDef_dtor(CSpecialAbilitiesForcePushDef* def);

void* __fastcall CSpecialAbilitiesForcePushDef_scalar_deleting_destructor(CSpecialAbilitiesForcePushDef* def, int edx, unsigned int flags)
{
    (void)edx;
    CSpecialAbilitiesForcePushDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}