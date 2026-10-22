struct CTCWallMountEffects {
    void* vftable;
};

void __fastcall CTCWallMountEffects_dtor(CTCWallMountEffects* effects);
void operator delete(void*);

void* __fastcall CTCWallMountEffects_scalar_deleting_destructor(CTCWallMountEffects* effects, int edx_unused, unsigned int flags)
{
    CTCWallMountEffects_dtor(effects);
    if (flags & 1)
        operator delete(effects);
    return effects;
}