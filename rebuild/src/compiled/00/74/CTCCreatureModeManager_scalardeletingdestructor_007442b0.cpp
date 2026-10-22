struct CTCCreatureModeManager {
    void* vftable;
};

void __fastcall CTCCreatureModeManager_dtor(CTCCreatureModeManager* manager);
void operator delete(void*);

void* __fastcall CTCCreatureModeManager_scalar_deleting_destructor(CTCCreatureModeManager* manager, int edx_unused, unsigned int flags)
{
    CTCCreatureModeManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}