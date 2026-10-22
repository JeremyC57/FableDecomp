struct CParticleUpdateParameters {
    void* vftable;
};

void __fastcall CParticleUpdateParameters_dtor(CParticleUpdateParameters* parameters);
void operator delete(void*);

void* __fastcall CParticleUpdateParameters_scalar_deleting_destructor(CParticleUpdateParameters* parameters, int edx_unused, unsigned int flags)
{
    CParticleUpdateParameters_dtor(parameters);
    if (flags & 1)
        operator delete(parameters);
    return parameters;
}