#pragma optimize("s", on)
class CCarryableDef
{ public:
    void* vftable;
};
extern void* const g_CCarryableDef_vftable;
extern void __fastcall CCarryableDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CCarryableDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CCarryableDef_vftable;
    CCarryableDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}