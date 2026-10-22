struct CPhysicsMesh {
    void* vftable;
};

void __fastcall CPhysicsMesh_dtor(CPhysicsMesh* mesh);
void operator delete(void*);

void* __fastcall CPhysicsMesh_scalar_deleting_destructor(CPhysicsMesh* mesh, int edx_unused, unsigned int flags)
{
    CPhysicsMesh_dtor(mesh);
    if (flags & 1)
        operator delete(mesh);
    return mesh;
}