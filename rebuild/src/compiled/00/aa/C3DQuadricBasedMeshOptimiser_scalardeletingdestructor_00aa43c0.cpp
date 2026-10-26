class C3DQuadricBasedMeshOptimiser { public:
    void* vftable;
};

void __fastcall C3DQuadricBasedMeshOptimiser_dtor(C3DQuadricBasedMeshOptimiser* optimiser);
void operator delete(void*);

void* __fastcall C3DQuadricBasedMeshOptimiser_scalar_deleting_destructor(C3DQuadricBasedMeshOptimiser* optimiser, int edx_unused, unsigned int flags)
{
    C3DQuadricBasedMeshOptimiser_dtor(optimiser);
    if (flags & 1)
        operator delete(optimiser);
    return optimiser;
}