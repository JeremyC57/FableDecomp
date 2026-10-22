struct C3DAnimationManager {
    void* vftable;
};

void __fastcall C3DAnimationManager_dtor(C3DAnimationManager* manager);
void operator delete(void*);

void* __fastcall C3DAnimationManager_scalar_deleting_destructor(C3DAnimationManager* manager, int edx_unused, unsigned int flags)
{
    C3DAnimationManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}