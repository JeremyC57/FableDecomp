struct C3DAnimationInterpolator {
    void* vftable;
};

void __fastcall C3DAnimationInterpolator_dtor(C3DAnimationInterpolator* interpolator);
void operator delete(void*);

void* __fastcall C3DAnimationInterpolator_scalar_deleting_destructor(C3DAnimationInterpolator* interpolator, int edx_unused, unsigned int flags)
{
    C3DAnimationInterpolator_dtor(interpolator);
    if (flags & 1)
        operator delete(interpolator);
    return interpolator;
}