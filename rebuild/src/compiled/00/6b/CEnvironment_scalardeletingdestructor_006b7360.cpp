struct CEnvironment {
    void* vftable;
};

void __fastcall CEnvironment_dtor(CEnvironment* environment);
void operator delete(void*);

void* __fastcall CEnvironment_scalar_deleting_destructor(CEnvironment* environment, int edx_unused, unsigned int flags)
{
    CEnvironment_dtor(environment);
    if (flags & 1)
        operator delete(environment);
    return environment;
}