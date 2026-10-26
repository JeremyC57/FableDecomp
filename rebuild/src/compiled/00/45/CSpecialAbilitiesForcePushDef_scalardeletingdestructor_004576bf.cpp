#pragma optimize("s",on)
class CSpecialAbilitiesForcePushDef { public:
    int f0;
};

extern void __fastcall CSpecialAbilitiesForcePushDef_dtor(CSpecialAbilitiesForcePushDef* def);
extern void __cdecl op_delete(void* p);

void* __fastcall CSpecialAbilitiesForcePushDef_scalar_deleting_destructor(CSpecialAbilitiesForcePushDef* def, int edx, unsigned int flags)
{
    (void)edx;
    CSpecialAbilitiesForcePushDef_dtor(def);
    if (flags & 1)
        op_delete(def);
    return def;
}