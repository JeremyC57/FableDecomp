struct CEngineInternalPrimitiveOcclusionBase {
    void* vftable;
};

void __fastcall CEngineInternalPrimitiveOcclusionBase_dtor(CEngineInternalPrimitiveOcclusionBase* occlusion);
void operator delete(void*);

void* __fastcall CEngineInternalPrimitiveOcclusionBase_scalar_deleting_destructor(CEngineInternalPrimitiveOcclusionBase* occlusion, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveOcclusionBase_dtor(occlusion);
    if (flags & 1)
        operator delete(occlusion);
    return occlusion;
}