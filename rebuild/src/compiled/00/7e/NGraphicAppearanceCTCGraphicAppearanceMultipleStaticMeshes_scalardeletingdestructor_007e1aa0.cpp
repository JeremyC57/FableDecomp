struct CTCGraphicAppearanceMultipleStaticMeshes {
    void* vftable;
};

void __fastcall CTCGraphicAppearanceMultipleStaticMeshes_dtor(CTCGraphicAppearanceMultipleStaticMeshes* meshes);
void operator delete(void*);

void* __fastcall CTCGraphicAppearanceMultipleStaticMeshes_scalar_deleting_destructor(CTCGraphicAppearanceMultipleStaticMeshes* meshes, int edx_unused, unsigned int flags)
{
    CTCGraphicAppearanceMultipleStaticMeshes_dtor(meshes);
    if (flags & 1)
        operator delete(meshes);
    return meshes;
}