#pragma optimize("s", on)
class CLookDef
{ public:
    void* vftable;
};
extern void* const g_CLookDef_vftable;
extern void __fastcall CLookDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CLookDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CLookDef_vftable;
    CLookDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}