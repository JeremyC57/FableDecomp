struct CMeshData {
    void* vftable;
};

void __fastcall CMeshData_dtor_00a2b6f0(CMeshData* mesh);
void operator delete(void*);

void* __fastcall CMeshData_scalar_deleting_destructor(CMeshData* mesh, int edx_unused, unsigned int flags)
{
    CMeshData_dtor_00a2b6f0(mesh);
    if (flags & 1)
        operator delete(mesh);
    return mesh;
}