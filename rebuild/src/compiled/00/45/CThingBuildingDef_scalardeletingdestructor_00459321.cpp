#pragma optimize("s", on)
extern void __fastcall CThingBuildingDef_Destruct(void* def);
void* __fastcall CThingBuildingDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CThingBuildingDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}