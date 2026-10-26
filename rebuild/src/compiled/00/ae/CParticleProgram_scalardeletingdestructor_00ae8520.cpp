class CParticleProgram { public:
    void* vftable;
};

void __fastcall CParticleProgram_dtor(CParticleProgram* program);
void operator delete(void*);

void* __fastcall CParticleProgram_scalar_deleting_destructor(CParticleProgram* program, int edx_unused, unsigned int flags)
{
    CParticleProgram_dtor(program);
    if (flags & 1)
        operator delete(program);
    return program;
}