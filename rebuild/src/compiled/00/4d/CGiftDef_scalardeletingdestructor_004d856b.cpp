#pragma optimize("s", on)
struct CGiftDef
{
    void* vftable;
};
extern void* const g_CGiftDef_vftable;
extern void __fastcall CGiftDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CGiftDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CGiftDef_vftable;
    CGiftDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}