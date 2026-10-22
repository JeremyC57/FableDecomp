struct CEnginePrimitive2DViewportManager {
    void* vftable;
};

void __fastcall CEnginePrimitive2DViewportManager_dtor(CEnginePrimitive2DViewportManager* manager);
void operator delete(void*);

void* __fastcall CEnginePrimitive2DViewportManager_scalar_deleting_destructor(CEnginePrimitive2DViewportManager* manager, int edx_unused, unsigned int flags)
{
    CEnginePrimitive2DViewportManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}