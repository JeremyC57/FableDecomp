struct CEngineLandscapeMemoryManager {
    void* vftable;
};

void __fastcall CEngineLandscapeMemoryManager_dtor(CEngineLandscapeMemoryManager* manager);
void operator delete(void*);

void* __fastcall CEngineLandscapeMemoryManager_scalar_deleting_destructor(CEngineLandscapeMemoryManager* manager, int edx_unused, unsigned int flags)
{
    CEngineLandscapeMemoryManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}