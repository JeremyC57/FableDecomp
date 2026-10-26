#pragma optimize("s",on)
extern "C" void __fastcall CTattooDef_dtor(void* def);

void* __fastcall CTattooDef_scalar_deleting_destructor(void* def, int edx, unsigned int flags)
{
    (void)edx;
    CTattooDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}