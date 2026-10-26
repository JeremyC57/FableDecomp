#pragma optimize("s", on)
extern void __fastcall CPlayerInventoryDef_Destruct(void* def);
void* __fastcall CPlayerInventoryDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CPlayerInventoryDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}