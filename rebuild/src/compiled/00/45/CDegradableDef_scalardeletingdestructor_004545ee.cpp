#pragma optimize("s",on)
extern void __fastcall CDegradableDef_dtor(void* def);
extern void __cdecl operator_delete(void* p);

void* __fastcall CDegradableDef_scalar_deleting_destructor(void* def, int edx_dummy, unsigned int flags)
{
    CDegradableDef_dtor(def);
    if (flags & 1)
        operator_delete(def);
    return def;
}