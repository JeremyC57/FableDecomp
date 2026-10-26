#pragma optimize("s", on)
class CTavernTableDef
{ public:
    void* vftable;
};
extern void* const g_CTavernTableDef_vftable;
extern void __fastcall CTavernTableDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTavernTableDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CTavernTableDef_vftable;
    CTavernTableDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}