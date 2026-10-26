#pragma optimize("s",on)
class CMaterialDef { public: int f0; };
extern void __fastcall CMaterialDef_dtor(CMaterialDef* def);

void* __fastcall CMaterialDef_scalar_deleting_destructor(CMaterialDef* def, int edx_dummy, unsigned int flags)
{
    (void)edx_dummy;
    CMaterialDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}