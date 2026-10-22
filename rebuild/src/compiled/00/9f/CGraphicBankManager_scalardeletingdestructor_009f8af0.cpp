struct CGraphicBankManager {
    void* vftable;
};

void __fastcall CGraphicBankManager_dtor(CGraphicBankManager* manager);
void operator delete(void*);

void* __fastcall CGraphicBankManager_scalar_deleting_destructor(CGraphicBankManager* manager, int edx_unused, unsigned int flags)
{
    CGraphicBankManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}