#pragma optimize("s", on)
struct CStealthDef
{
    void* vftable;
};
extern void* const g_CStealthDef_vftable;
extern void __fastcall CStealthDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CStealthDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CStealthDef_vftable;
    CStealthDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}