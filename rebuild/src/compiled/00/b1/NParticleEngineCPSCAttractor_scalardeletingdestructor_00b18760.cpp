struct CPSCAttractor {
    void* vftable;
};

void __fastcall CPSCAttractor_dtor(CPSCAttractor* attractor);
void operator delete(void*);

void* __fastcall CPSCAttractor_scalar_deleting_destructor(CPSCAttractor* attractor, int edx_unused, unsigned int flags)
{
    CPSCAttractor_dtor(attractor);
    if (flags & 1)
        operator delete(attractor);
    return attractor;
}