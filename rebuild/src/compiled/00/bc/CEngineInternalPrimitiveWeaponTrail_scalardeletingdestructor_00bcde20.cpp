struct CEngineInternalPrimitiveWeaponTrail {
    void* vftable;
};

void __fastcall CEngineInternalPrimitiveWeaponTrail_dtor(CEngineInternalPrimitiveWeaponTrail* trail);
void operator delete(void*);

void* __fastcall CEngineInternalPrimitiveWeaponTrail_scalar_deleting_destructor(CEngineInternalPrimitiveWeaponTrail* trail, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveWeaponTrail_dtor(trail);
    if (flags & 1)
        operator delete(trail);
    return trail;
}