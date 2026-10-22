struct CEngineOcclusionManager {
    void* vftable;
};

void __fastcall CEngineOcclusionManager_dtor(CEngineOcclusionManager* manager);
void operator delete(void*);

void* __fastcall CEngineOcclusionManager_scalar_deleting_destructor(CEngineOcclusionManager* manager, int edx_unused, unsigned int flags)
{
    CEngineOcclusionManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}