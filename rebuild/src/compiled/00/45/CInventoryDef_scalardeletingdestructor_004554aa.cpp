#pragma optimize("s",on)
class CInventoryDef;
extern void __fastcall CInventoryDef_dtor(CInventoryDef* def);

void* __fastcall CInventoryDef_scalar_deleting_destructor(CInventoryDef* def, int, unsigned int flags)
{
    CInventoryDef_dtor(def);
    if (flags & 1)
        operator delete((void*)def);
    return def;
}