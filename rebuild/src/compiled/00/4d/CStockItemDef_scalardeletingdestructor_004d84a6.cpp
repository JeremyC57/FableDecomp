#pragma optimize("s", on)
class CStockItemDef
{ public:
    void* vftable;
};
extern void* const g_CStockItemDef_vftable;
extern void __fastcall CStockItemDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CStockItemDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CStockItemDef_vftable;
    CStockItemDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}