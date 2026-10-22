struct CTCPhysicsControlled {
    void* vftable;
};

void __fastcall CTCPhysicsControlled_dtor(CTCPhysicsControlled* controlled);
void operator delete(void*);

void* __fastcall CTCPhysicsControlled_scalar_deleting_destructor(CTCPhysicsControlled* controlled, int edx_unused, unsigned int flags)
{
    CTCPhysicsControlled_dtor(controlled);
    if (flags & 1)
        operator delete(controlled);
    return controlled;
}