#pragma optimize("s", on)
extern void __fastcall CThingMarkerDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CThingMarkerDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CThingMarkerDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}