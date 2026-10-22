#pragma optimize("s", on)
extern void __fastcall CPlayerInventoryDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CPlayerInventoryDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CPlayerInventoryDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}