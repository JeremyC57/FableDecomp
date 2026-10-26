class CParticleContext { public:
    void* vftable;
};

void __fastcall CParticleContext_dtor(CParticleContext* context);
void operator delete(void*);

void* __fastcall CParticleContext_scalar_deleting_destructor(CParticleContext* context, int edx_unused, unsigned int flags)
{
    CParticleContext_dtor(context);
    if (flags & 1)
        operator delete(context);
    return context;
}