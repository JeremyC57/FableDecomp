#pragma optimize("s", on)
class CMultiStaticMeshEntryDef
{ public:
    void* vftable;
};
extern void* const g_CMultiStaticMeshEntryDef_vftable;
extern void __fastcall CMultiStaticMeshEntryDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CMultiStaticMeshEntryDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CMultiStaticMeshEntryDef_vftable;
    CMultiStaticMeshEntryDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}