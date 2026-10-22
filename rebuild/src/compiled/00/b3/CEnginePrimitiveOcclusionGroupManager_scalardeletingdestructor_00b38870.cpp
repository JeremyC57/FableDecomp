struct CEnginePrimitiveOcclusionGroupManager {
    void* vftable;
};

void __fastcall CEnginePrimitiveOcclusionGroupManager_dtor(CEnginePrimitiveOcclusionGroupManager* manager);
void operator delete(void*);

void* __fastcall CEnginePrimitiveOcclusionGroupManager_scalar_deleting_destructor(CEnginePrimitiveOcclusionGroupManager* manager, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveOcclusionGroupManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}