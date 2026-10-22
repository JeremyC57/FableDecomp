struct CASoundManager {
    void* vftable;
};

void __fastcall CASoundManager_dtor(CASoundManager* manager);
void operator delete(void*);

void* __fastcall CASoundManager_scalar_deleting_destructor(CASoundManager* manager, int edx_unused, unsigned int flags)
{
    CASoundManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}