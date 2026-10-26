#pragma optimize("s", on)
class CLightningOrbDef
{ public:
    void* vftable;
};
extern void* const g_CLightningOrbDef_vftable;
extern void __fastcall CLightningOrbDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CLightningOrbDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CLightningOrbDef_vftable;
    CLightningOrbDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}