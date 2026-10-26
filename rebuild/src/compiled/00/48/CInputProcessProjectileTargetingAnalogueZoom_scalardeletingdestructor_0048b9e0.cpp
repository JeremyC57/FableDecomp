class CInputProcessProjectileTargetingAnalogueZoom { public:
    void* vftable;
};

void __fastcall CInputProcessProjectileTargetingAnalogueZoom_dtor(CInputProcessProjectileTargetingAnalogueZoom* process);
void operator delete(void*);

void* __fastcall CInputProcessProjectileTargetingAnalogueZoom_scalar_deleting_destructor(CInputProcessProjectileTargetingAnalogueZoom* process, int edx_unused, unsigned int flags)
{
    CInputProcessProjectileTargetingAnalogueZoom_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}