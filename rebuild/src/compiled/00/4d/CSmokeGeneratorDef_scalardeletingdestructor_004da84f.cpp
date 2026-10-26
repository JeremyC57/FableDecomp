#pragma optimize("s", on)
class CSmokeGeneratorDef
{ public:
    void* vftable;
};
extern void* const g_CSmokeGeneratorDef_vftable;
extern void __fastcall CSmokeGeneratorDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CSmokeGeneratorDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CSmokeGeneratorDef_vftable;
    CSmokeGeneratorDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}