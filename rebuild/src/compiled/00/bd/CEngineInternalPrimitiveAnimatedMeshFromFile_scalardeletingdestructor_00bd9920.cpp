struct CEngineInternalPrimitiveAnimatedMeshFromFile {
    void* vftable;
};

void __fastcall CEngineInternalPrimitiveAnimatedMeshFromFile_dtor(CEngineInternalPrimitiveAnimatedMeshFromFile* file);
void operator delete(void*);

void* __fastcall CEngineInternalPrimitiveAnimatedMeshFromFile_scalar_deleting_destructor(CEngineInternalPrimitiveAnimatedMeshFromFile* file, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveAnimatedMeshFromFile_dtor(file);
    if (flags & 1)
        operator delete(file);
    return file;
}