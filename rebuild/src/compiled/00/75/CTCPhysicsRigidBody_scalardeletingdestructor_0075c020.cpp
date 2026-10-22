struct CTCPhysicsRigidBody {
    void* vftable;
};

void __fastcall CTCPhysicsRigidBody_dtor(CTCPhysicsRigidBody* body);
void operator delete(void*);

void* __fastcall CTCPhysicsRigidBody_scalar_deleting_destructor(CTCPhysicsRigidBody* body, int edx_unused, unsigned int flags)
{
    CTCPhysicsRigidBody_dtor(body);
    if (flags & 1)
        operator delete(body);
    return body;
}