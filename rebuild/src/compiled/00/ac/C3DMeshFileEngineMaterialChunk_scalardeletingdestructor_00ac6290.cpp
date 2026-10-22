struct C3DMeshFileEngineMaterialChunk {
    void* vftable;
};

void __fastcall C3DMeshFileEngineMaterialChunk_dtor(C3DMeshFileEngineMaterialChunk* chunk);
void operator delete(void*);

void* __fastcall C3DMeshFileEngineMaterialChunk_scalar_deleting_destructor(C3DMeshFileEngineMaterialChunk* chunk, int edx_unused, unsigned int flags)
{
    C3DMeshFileEngineMaterialChunk_dtor(chunk);
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}