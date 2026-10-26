class CThingManager { public:
    void* vftable;
};

void __fastcall CThingManager_dtor(CThingManager* manager);
void operator delete(void*);

void* __fastcall CThingManager_scalar_deleting_destructor(CThingManager* manager, int edx_unused, unsigned int flags)
{
    CThingManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}