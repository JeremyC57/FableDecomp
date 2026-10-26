#pragma optimize("s", on)
class CDoorDef
{ public:
    void* vftable;
};
extern void* const g_CDoorDef_vftable;
extern void __fastcall CDoorDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CDoorDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CDoorDef_vftable;
    CDoorDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}