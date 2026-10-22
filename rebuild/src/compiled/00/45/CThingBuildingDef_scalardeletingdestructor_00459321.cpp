#pragma optimize("s", on)
extern void __fastcall CThingBuildingDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CThingBuildingDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CThingBuildingDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}