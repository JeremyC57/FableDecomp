#pragma optimize("s",on)
class CMaterialDef { public: int f0; };
extern void __fastcall CMaterialDef_dtor(CMaterialDef* def);
extern void __cdecl engine_operator_delete(void* p);

void* __fastcall CMaterialDef_scalar_deleting_destructor(CMaterialDef* def, int edx_dummy, unsigned int flags)
{
    (void)edx_dummy;
    CMaterialDef_dtor(def);
    if (flags & 1)
        engine_operator_delete(def);
    return def;
}