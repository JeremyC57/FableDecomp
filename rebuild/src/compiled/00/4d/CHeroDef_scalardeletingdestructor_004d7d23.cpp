#pragma optimize("s", on)
struct CHeroDef
{
    void* vftable;
};
extern void* const g_CHeroDef_vftable;
extern void __fastcall CHeroDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CHeroDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CHeroDef_vftable;
    CHeroDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}