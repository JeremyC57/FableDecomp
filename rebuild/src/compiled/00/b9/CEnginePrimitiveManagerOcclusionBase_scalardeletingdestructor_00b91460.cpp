struct CEnginePrimitiveManagerOcclusionBase {
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerOcclusionBase_dtor(CEnginePrimitiveManagerOcclusionBase* occlusion);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerOcclusionBase_scalar_deleting_destructor(CEnginePrimitiveManagerOcclusionBase* occlusion, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerOcclusionBase_dtor(occlusion);
    if (flags & 1)
        operator delete(occlusion);
    return occlusion;
}