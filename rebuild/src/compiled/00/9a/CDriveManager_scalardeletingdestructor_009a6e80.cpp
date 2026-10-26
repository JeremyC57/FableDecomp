class CDriveManager { public:
    void* vftable;
};

void __fastcall CDriveManager_dtor(CDriveManager* manager);
void operator delete(void*);

void* __fastcall CDriveManager_scalar_deleting_destructor(CDriveManager* manager, int edx_unused, unsigned int flags)
{
    CDriveManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}