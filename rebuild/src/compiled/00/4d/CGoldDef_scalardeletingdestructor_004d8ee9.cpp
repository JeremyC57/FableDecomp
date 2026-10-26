#pragma optimize("s", on)
class CGoldDef
{ public:
    void* vftable;
};
extern void* const g_CGoldDef_vftable;
extern void __fastcall CGoldDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CGoldDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CGoldDef_vftable;
    CGoldDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}