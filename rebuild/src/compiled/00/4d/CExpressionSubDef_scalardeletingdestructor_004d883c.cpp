#pragma optimize("s", on)
struct CExpressionSubDef
{
    void* vftable;
};
extern void* const g_CExpressionSubDef_vftable;
extern void __fastcall CExpressionSubDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CExpressionSubDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CExpressionSubDef_vftable;
    CExpressionSubDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}