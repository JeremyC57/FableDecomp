#pragma optimize("s",on)
extern "C" void __fastcall CTattooDef_dtor(void* def);
extern "C" void __cdecl fable_operator_delete(void* p);

void* __fastcall CTattooDef_scalar_deleting_destructor(void* def, int edx, unsigned int flags)
{
    (void)edx;
    CTattooDef_dtor(def);
    if (flags & 1)
        fable_operator_delete(def);
    return def;
}