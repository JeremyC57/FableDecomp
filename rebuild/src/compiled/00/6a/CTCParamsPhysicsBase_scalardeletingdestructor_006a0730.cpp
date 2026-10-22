struct CTCParamsPhysicsBase {
    void* vftable;
};

void __fastcall CTCParamsPhysicsBase_dtor(CTCParamsPhysicsBase* physics);
void operator delete(void*);

void* __fastcall CTCParamsPhysicsBase_scalar_deleting_destructor(CTCParamsPhysicsBase* physics, int edx_unused, unsigned int flags)
{
    CTCParamsPhysicsBase_dtor(physics);
    if (flags & 1)
        operator delete(physics);
    return physics;
}