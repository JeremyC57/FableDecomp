struct CEngineResourceManager {
    void* vftable;
};

void __fastcall CEngineResourceManager_dtor(CEngineResourceManager* manager);
void operator delete(void*);

void* __fastcall CEngineResourceManager_scalar_deleting_destructor(CEngineResourceManager* manager, int edx_unused, unsigned int flags)
{
    CEngineResourceManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}