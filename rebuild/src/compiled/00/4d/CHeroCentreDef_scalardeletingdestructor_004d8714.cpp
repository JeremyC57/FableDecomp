#pragma optimize("s", on)
class CHeroCentreDef
{ public:
    void* vftable;
};
extern void* const g_CHeroCentreDef_vftable;
extern void __fastcall CHeroCentreDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CHeroCentreDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CHeroCentreDef_vftable;
    CHeroCentreDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}