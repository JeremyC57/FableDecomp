#pragma optimize("s",on)
extern void __fastcall CPhysicsDef_dtor(void* def);
extern void __cdecl op_delete(void* p);
extern void* const CPhysicsDef_vftable;

struct CPhysicsDef {
    void* vftable;
};

void* __fastcall CPhysicsDef_scalar_deleting_destructor(CPhysicsDef* def, int edxpad, unsigned int flags)
{
    (void)edxpad;
    def->vftable = (void*)&CPhysicsDef_vftable;
    CPhysicsDef_dtor(def);
    if (flags & 1) {
        op_delete(def);
    }
    return def;
}