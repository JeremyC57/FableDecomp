class CGameScriptThingManager { public:
    void* vftable;
};

void __fastcall CGameScriptThingManager_dtor(CGameScriptThingManager* manager);
void operator delete(void*);

void* __fastcall CGameScriptThingManager_scalar_deleting_destructor(CGameScriptThingManager* manager, int edx_unused, unsigned int flags)
{
    CGameScriptThingManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}