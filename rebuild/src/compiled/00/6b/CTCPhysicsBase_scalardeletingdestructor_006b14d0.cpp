struct CTCPhysicsBase {
    void* vftable;
};

void __fastcall CTCPhysicsBase_dtor(CTCPhysicsBase* physics);
void operator delete(void*);

void* __fastcall CTCPhysicsBase_scalar_deleting_destructor(CTCPhysicsBase* physics, int edx_unused, unsigned int flags)
{
    CTCPhysicsBase_dtor(physics);
    if (flags & 1)
        operator delete(physics);
    return physics;
}