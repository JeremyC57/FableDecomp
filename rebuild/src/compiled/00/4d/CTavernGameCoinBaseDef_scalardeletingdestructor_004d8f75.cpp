#pragma optimize("s", on)
struct CTavernGameCoinBaseDef
{
    void* vftable;
};
extern void* const g_CTavernGameCoinBaseDef_vftable;
extern void __fastcall CTavernGameCoinBaseDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTavernGameCoinBaseDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CTavernGameCoinBaseDef_vftable;
    CTavernGameCoinBaseDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}