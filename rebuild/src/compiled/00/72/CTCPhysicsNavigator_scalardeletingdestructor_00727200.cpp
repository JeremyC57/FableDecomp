struct CTCPhysicsNavigator {
    void* vftable;
};

void __fastcall CTCPhysicsNavigator_dtor(CTCPhysicsNavigator* navigator);
void operator delete(void*);

void* __fastcall CTCPhysicsNavigator_scalar_deleting_destructor(CTCPhysicsNavigator* navigator, int edx_unused, unsigned int flags)
{
    CTCPhysicsNavigator_dtor(navigator);
    if (flags & 1)
        operator delete(navigator);
    return navigator;
}