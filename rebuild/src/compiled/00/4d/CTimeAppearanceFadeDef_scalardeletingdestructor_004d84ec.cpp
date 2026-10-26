#pragma optimize("s", on)
class CTimeAppearanceFadeDef
{ public:
    void* vftable;
};
extern void* const g_CTimeAppearanceFadeDef_vftable;
extern void __fastcall CTimeAppearanceFadeDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTimeAppearanceFadeDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CTimeAppearanceFadeDef_vftable;
    CTimeAppearanceFadeDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}