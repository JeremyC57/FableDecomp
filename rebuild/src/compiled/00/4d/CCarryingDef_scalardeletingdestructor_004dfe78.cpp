#pragma optimize("s", on)
extern void __fastcall CCarryingDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CCarryingDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CCarryingDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}