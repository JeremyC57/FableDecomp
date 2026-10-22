struct C3DMesh2 {
    void* vftable;
};

void __fastcall C3DMesh2_dtor(C3DMesh2* mesh2);
void operator delete(void*);

void* __fastcall C3DMesh2_scalar_deleting_destructor(C3DMesh2* mesh2, int edx_unused, unsigned int flags)
{
    C3DMesh2_dtor(mesh2);
    if (flags & 1)
        operator delete(mesh2);
    return mesh2;
}