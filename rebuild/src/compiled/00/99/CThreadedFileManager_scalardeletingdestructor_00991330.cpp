struct CThreadedFileManager {
    void* vftable;
};

void __fastcall CThreadedFileManager_dtor(CThreadedFileManager* manager);
void operator delete(void*);

void* __fastcall CThreadedFileManager_scalar_deleting_destructor(CThreadedFileManager* manager, int edx_unused, unsigned int flags)
{
    CThreadedFileManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}