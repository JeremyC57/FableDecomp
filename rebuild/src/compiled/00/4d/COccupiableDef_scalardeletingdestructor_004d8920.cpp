#pragma optimize("s", on)
class COccupiableDef
{ public:
    void* vftable;
};
extern void* const g_COccupiableDef_vftable;
extern void __fastcall COccupiableDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(COccupiableDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_COccupiableDef_vftable;
    COccupiableDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}