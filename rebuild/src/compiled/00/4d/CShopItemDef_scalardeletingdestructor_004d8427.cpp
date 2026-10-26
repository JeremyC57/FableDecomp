#pragma optimize("s", on)
class CShopItemDef
{ public:
    void* vftable;
};
extern void* const g_CShopItemDef_vftable;
extern void __fastcall CShopItemDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CShopItemDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CShopItemDef_vftable;
    CShopItemDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}