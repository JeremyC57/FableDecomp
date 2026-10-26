#pragma optimize("s", on)
class CTavernGameCoinGolfDef
{ public:
    void* vftable;
};
extern void* const g_CTavernGameCoinGolfDef_vftable;
extern void __fastcall CTavernGameCoinGolfDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTavernGameCoinGolfDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CTavernGameCoinGolfDef_vftable;
    CTavernGameCoinGolfDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}