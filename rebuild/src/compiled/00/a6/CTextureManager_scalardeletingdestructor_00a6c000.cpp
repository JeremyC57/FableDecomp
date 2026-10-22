struct CTextureManager {
    void* vftable;
};

void __fastcall CTextureManager_dtor(CTextureManager* manager);
void operator delete(void*);

void* __fastcall CTextureManager_scalar_deleting_destructor(CTextureManager* manager, int edx_unused, unsigned int flags)
{
    CTextureManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}