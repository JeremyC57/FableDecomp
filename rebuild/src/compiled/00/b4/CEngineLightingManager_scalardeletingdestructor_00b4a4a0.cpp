struct CEngineLightingManager {
    void* vftable;
};

void __fastcall CEngineLightingManager_dtor(CEngineLightingManager* manager);
void operator delete(void*);

void* __fastcall CEngineLightingManager_scalar_deleting_destructor(CEngineLightingManager* manager, int edx_unused, unsigned int flags)
{
    CEngineLightingManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}