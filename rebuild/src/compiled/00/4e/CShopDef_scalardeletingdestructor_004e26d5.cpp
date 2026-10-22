#pragma optimize("s", on)
extern void __fastcall CShopDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CShopDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CShopDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}