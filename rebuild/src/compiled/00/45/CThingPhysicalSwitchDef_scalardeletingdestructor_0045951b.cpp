#pragma optimize("s", on)
extern void __fastcall CThingPhysicalSwitchDef_Destruct(void* def);
void* __fastcall CThingPhysicalSwitchDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CThingPhysicalSwitchDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}