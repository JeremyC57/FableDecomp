#pragma optimize("s", on)
extern void __fastcall CThingGroupDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CThingGroupDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CThingGroupDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}