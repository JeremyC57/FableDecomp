struct CDisplayManager {
// Leaf: `scalar_deleting_destructor'
    void* vftable;
};

void __fastcall CDisplayManager_dtor(CDisplayManager* manager);
extern void free_engine(void* p);

void* __fastcall CDisplayManager_scalar_deleting_destructor(CDisplayManager* manager, int edx_unused, unsigned int flags)
{
    CDisplayManager_dtor(manager);
    if (flags & 1)
        free_engine(manager);
    return manager;
}