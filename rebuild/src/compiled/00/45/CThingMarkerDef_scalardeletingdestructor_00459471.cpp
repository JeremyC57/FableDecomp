#pragma optimize("s", on)
extern void __fastcall CThingMarkerDef_Destruct(void* def);
void* __fastcall CThingMarkerDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CThingMarkerDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}