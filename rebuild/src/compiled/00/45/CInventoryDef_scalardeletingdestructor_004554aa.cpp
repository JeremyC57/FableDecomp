#pragma optimize("s",on)
struct CInventoryDef;
extern void __fastcall CInventoryDef_dtor(CInventoryDef* def);
extern void __cdecl op_delete(void* p);

void* __fastcall CInventoryDef_scalar_deleting_destructor(CInventoryDef* def, int, unsigned int flags)
{
    CInventoryDef_dtor(def);
    if (flags & 1)
        op_delete((void*)def);
    return def;
}