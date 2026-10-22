struct CEnginePrimitiveOcclusionManager {
    void* vftable;
};

void __fastcall CEnginePrimitiveOcclusionManager_dtor(CEnginePrimitiveOcclusionManager* manager);
void operator delete(void*);

void* __fastcall CEnginePrimitiveOcclusionManager_scalar_deleting_destructor(CEnginePrimitiveOcclusionManager* manager, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveOcclusionManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}