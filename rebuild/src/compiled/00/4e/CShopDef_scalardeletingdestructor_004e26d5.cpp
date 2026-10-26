#pragma optimize("s", on)
extern void __fastcall CShopDef_Destruct(void* def);
void* __fastcall CShopDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CShopDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}