#pragma optimize("s", on)
class CCrateStackDef
{ public:
    void* vftable;
};
extern void* const g_CCrateStackDef_vftable;
extern void __fastcall CCrateStackDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CCrateStackDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CCrateStackDef_vftable;
    CCrateStackDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}