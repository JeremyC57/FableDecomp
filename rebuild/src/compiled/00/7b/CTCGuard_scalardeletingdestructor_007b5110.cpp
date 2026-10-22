struct CTCGuard {
    void* vftable;
};

void __fastcall CTCGuard_dtor(CTCGuard* guard);
void operator delete(void*);

void* __fastcall CTCGuard_scalar_deleting_destructor(CTCGuard* guard, int edx_unused, unsigned int flags)
{
    CTCGuard_dtor(guard);
    if (flags & 1)
        operator delete(guard);
    return guard;
}