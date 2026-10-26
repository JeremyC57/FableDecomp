class CEnginePrimitiveGroupManager { public:
    void* vftable;
};

void __fastcall CEnginePrimitiveGroupManager_dtor(CEnginePrimitiveGroupManager* manager);
void operator delete(void*);

void* __fastcall CEnginePrimitiveGroupManager_scalar_deleting_destructor(CEnginePrimitiveGroupManager* manager, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveGroupManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}