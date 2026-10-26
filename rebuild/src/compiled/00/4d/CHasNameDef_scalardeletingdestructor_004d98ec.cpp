#pragma optimize("s", on)
class CHasNameDef
{ public:
    void* vftable;
};
extern void* const g_CHasNameDef_vftable;
extern void __fastcall CHasNameDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CHasNameDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CHasNameDef_vftable;
    CHasNameDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}