struct CEnginePrimitiveRendererSharedData {
    void* vftable;
};

void __fastcall CEnginePrimitiveRendererSharedData_dtor(CEnginePrimitiveRendererSharedData* shared);
void operator delete(void*);

void* __fastcall CEnginePrimitiveRendererSharedData_scalar_deleting_destructor(CEnginePrimitiveRendererSharedData* shared, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveRendererSharedData_dtor(shared);
    if (flags & 1)
        operator delete(shared);
    return shared;
}