#pragma optimize("s", on)
class CWallMountEffectsDef
{ public:
    void* vftable;
};
extern void* const g_CWallMountEffectsDef_vftable;
extern void __fastcall CWallMountEffectsDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CWallMountEffectsDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CWallMountEffectsDef_vftable;
    CWallMountEffectsDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}