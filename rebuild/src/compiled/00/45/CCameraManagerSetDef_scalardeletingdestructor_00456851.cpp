#pragma optimize("s",on)
extern void __fastcall CCameraManagerSetDef_dtor(void* def);
extern void __cdecl op_delete(void* p);

void* __fastcall CCameraManagerSetDef_scalar_deleting_destructor(void* def, int /*edx*/, unsigned int flags)
{
    CCameraManagerSetDef_dtor(def);
    if (flags & 1)
        op_delete(def);
    return def;
}