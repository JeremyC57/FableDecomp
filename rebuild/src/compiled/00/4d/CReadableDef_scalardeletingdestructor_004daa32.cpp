#pragma optimize("s", on)
class CReadableDef
{ public:
    void* vftable;
};
extern void* const g_CReadableDef_vftable;
extern void __fastcall CReadableDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CReadableDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CReadableDef_vftable;
    CReadableDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}