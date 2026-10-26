#pragma optimize("s", on)
class CSummonableCreatureDef
{ public:
    void* vftable;
};
extern void* const g_CSummonableCreatureDef_vftable;
extern void __fastcall CSummonableCreatureDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CSummonableCreatureDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CSummonableCreatureDef_vftable;
    CSummonableCreatureDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}