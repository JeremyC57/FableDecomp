#pragma optimize("s", on)
struct CVillageMemberDef
{
    void* vftable;
};
extern void* const g_CVillageMemberDef_vftable;
extern void __fastcall CVillageMemberDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CVillageMemberDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CVillageMemberDef_vftable;
    CVillageMemberDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}