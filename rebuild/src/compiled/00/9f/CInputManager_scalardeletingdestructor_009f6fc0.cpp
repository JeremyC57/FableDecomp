struct CInputManager {
    void* vftable;
};

void __fastcall CInputManager_dtor(CInputManager* manager);
void operator delete(void*);

void* __fastcall CInputManager_scalar_deleting_destructor(CInputManager* manager, int edx_unused, unsigned int flags)
{
    CInputManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}