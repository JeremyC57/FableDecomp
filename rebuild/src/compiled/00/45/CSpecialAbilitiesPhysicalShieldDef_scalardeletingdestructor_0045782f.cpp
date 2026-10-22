#pragma optimize("s",on)
struct CSpecialAbilitiesPhysicalShieldDef {
    void* vftable;
};

extern "C" void __fastcall CSpecialAbilitiesPhysicalShieldDef_scalar_dtor(CSpecialAbilitiesPhysicalShieldDef* def);
extern "C" void __cdecl op_delete(void* p);

void* __fastcall CSpecialAbilitiesPhysicalShieldDef_scalar_deleting_destructor(CSpecialAbilitiesPhysicalShieldDef* def, int edx_dummy, unsigned int flags)
{
    (void)edx_dummy;
    CSpecialAbilitiesPhysicalShieldDef_scalar_dtor(def);
    if (flags & 1)
        op_delete(def);
    return def;
}