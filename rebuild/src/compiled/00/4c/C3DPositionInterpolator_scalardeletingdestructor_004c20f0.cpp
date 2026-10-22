struct C3DPositionInterpolator {
    void* vftable;
};

void __fastcall C3DPositionInterpolator_dtor(C3DPositionInterpolator* interpolator);
void operator delete(void*);

void* __fastcall C3DPositionInterpolator_scalar_deleting_destructor(C3DPositionInterpolator* interpolator, int edx_unused, unsigned int flags)
{
    C3DPositionInterpolator_dtor(interpolator);
    if (flags & 1)
        operator delete(interpolator);
    return interpolator;
}