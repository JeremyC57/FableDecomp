struct CDefinitionManager {
    void* vftable;
};

void __fastcall CDefinitionManager_dtor(CDefinitionManager* manager);
void operator delete(void*);

void* __fastcall CDefinitionManager_scalar_deleting_destructor(CDefinitionManager* manager, int edx_unused, unsigned int flags)
{
    CDefinitionManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}