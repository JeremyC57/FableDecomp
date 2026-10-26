class CFontManager { public:
    void* vftable;
};

void __fastcall CFontManager_dtor(CFontManager* manager);
void operator delete(void*);

void* __fastcall CFontManager_scalar_deleting_destructor(CFontManager* manager, int edx_unused, unsigned int flags)
{
    CFontManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}