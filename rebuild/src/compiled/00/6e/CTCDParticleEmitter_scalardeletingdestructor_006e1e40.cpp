class CTCDParticleEmitter { public:
    void* vftable;
};

void __fastcall CTCDParticleEmitter_dtor(CTCDParticleEmitter* emitter);
void operator delete(void*);

void* __fastcall CTCDParticleEmitter_scalar_deleting_destructor(CTCDParticleEmitter* emitter, int edx_unused, unsigned int flags)
{
    CTCDParticleEmitter_dtor(emitter);
    if (flags & 1)
        operator delete(emitter);
    return emitter;
}