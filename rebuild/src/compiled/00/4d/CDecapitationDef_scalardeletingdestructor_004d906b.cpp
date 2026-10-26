#pragma optimize("s", on)
class CDecapitationDef
{ public:
    void* vftable;
};
extern void* const g_CDecapitationDef_vftable;
extern void __fastcall CDecapitationDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CDecapitationDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CDecapitationDef_vftable;
    CDecapitationDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}