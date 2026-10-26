#pragma optimize("s", on)
class CExplodingObjectDef
{ public:
    void* vftable;
};
extern void* const g_CExplodingObjectDef_vftable;
extern void __fastcall CExplodingObjectDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CExplodingObjectDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CExplodingObjectDef_vftable;
    CExplodingObjectDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}