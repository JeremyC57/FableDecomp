#pragma optimize("s", on)
extern void __fastcall CThingPhysicalSwitchDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CThingPhysicalSwitchDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CThingPhysicalSwitchDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}