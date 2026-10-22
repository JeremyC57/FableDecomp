struct CThingMapManager {
    void* vftable;
};

void __fastcall CThingMapManager_dtor(CThingMapManager* manager);
void operator delete(void*);

void* __fastcall CThingMapManager_scalar_deleting_destructor(CThingMapManager* manager, int edx_unused, unsigned int flags)
{
    CThingMapManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}