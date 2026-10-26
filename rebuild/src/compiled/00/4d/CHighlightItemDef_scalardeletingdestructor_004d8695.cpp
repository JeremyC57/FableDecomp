#pragma optimize("s", on)
class CHighlightItemDef
{ public:
    void* vftable;
};
extern void* const g_CHighlightItemDef_vftable;
extern void __fastcall CHighlightItemDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CHighlightItemDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CHighlightItemDef_vftable;
    CHighlightItemDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}