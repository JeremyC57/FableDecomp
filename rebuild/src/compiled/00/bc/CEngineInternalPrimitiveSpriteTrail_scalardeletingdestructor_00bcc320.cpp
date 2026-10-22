struct CEngineInternalPrimitiveSpriteTrail {
    void* vftable;
};

void __fastcall CEngineInternalPrimitiveSpriteTrail_dtor(CEngineInternalPrimitiveSpriteTrail* trail);
void operator delete(void*);

void* __fastcall CEngineInternalPrimitiveSpriteTrail_scalar_deleting_destructor(CEngineInternalPrimitiveSpriteTrail* trail, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveSpriteTrail_dtor(trail);
    if (flags & 1)
        operator delete(trail);
    return trail;
}