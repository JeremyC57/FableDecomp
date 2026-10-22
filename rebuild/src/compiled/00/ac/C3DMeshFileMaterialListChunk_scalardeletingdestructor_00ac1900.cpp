struct C3DMeshFileMaterialListChunk {
    void* vftable;
};

void __fastcall C3DMeshFileMaterialListChunk_dtor(C3DMeshFileMaterialListChunk* chunk);
void operator delete(void*);

void* __fastcall C3DMeshFileMaterialListChunk_scalar_deleting_destructor(C3DMeshFileMaterialListChunk* chunk, int edx_unused, unsigned int flags)
{
    C3DMeshFileMaterialListChunk_dtor(chunk);
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}