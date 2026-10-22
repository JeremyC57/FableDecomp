#pragma optimize("s", on)
struct CKickableDef
{
    void* vftable;
};
extern void* const g_CKickableDef_vftable;
extern void __fastcall CKickableDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CKickableDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CKickableDef_vftable;
    CKickableDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}