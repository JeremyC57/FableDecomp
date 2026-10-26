#pragma optimize("s", on)
class CWillResponseDef
{ public:
    void* vftable;
};
extern void* const g_CWillResponseDef_vftable;
extern void __fastcall CWillResponseDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CWillResponseDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CWillResponseDef_vftable;
    CWillResponseDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}