struct CEngineSimulationManager {
    void* vftable;
};

void __fastcall CEngineSimulationManager_dtor(CEngineSimulationManager* manager);
void operator delete(void*);

void* __fastcall CEngineSimulationManager_scalar_deleting_destructor(CEngineSimulationManager* manager, int edx_unused, unsigned int flags)
{
    CEngineSimulationManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}