#pragma optimize("s",on)
extern void __fastcall CDegradableDef_dtor(void* def);

void* __fastcall CDegradableDef_scalar_deleting_destructor(void* def, int edx_dummy, unsigned int flags)
{
    CDegradableDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}