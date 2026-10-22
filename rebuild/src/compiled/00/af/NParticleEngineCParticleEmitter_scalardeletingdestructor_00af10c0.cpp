struct CParticleEmitter {
    void* vftable;
};

void __fastcall CParticleEmitter_dtor(CParticleEmitter* emitter);
void operator delete(void*);

void* __fastcall CParticleEmitter_scalar_deleting_destructor(CParticleEmitter* emitter, int edx_unused, unsigned int flags)
{
    CParticleEmitter_dtor(emitter);
    if (flags & 1)
        operator delete(emitter);
    return emitter;
}