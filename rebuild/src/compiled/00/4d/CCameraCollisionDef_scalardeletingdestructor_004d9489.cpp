#pragma optimize("s", on)
class CCameraCollisionDef
{ public:
    void* vftable;
};
extern void* const g_CCameraCollisionDef_vftable;
extern void __fastcall CCameraCollisionDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CCameraCollisionDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CCameraCollisionDef_vftable;
    CCameraCollisionDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}