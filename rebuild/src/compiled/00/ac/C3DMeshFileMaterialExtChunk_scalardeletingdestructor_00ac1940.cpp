class C3DMeshFileMaterialExtChunk { public:
    void* vftable;
};

void __fastcall C3DMeshFileMaterialExtChunk_dtor(C3DMeshFileMaterialExtChunk* chunk);
void operator delete(void*);

void* __fastcall C3DMeshFileMaterialExtChunk_scalar_deleting_destructor(C3DMeshFileMaterialExtChunk* chunk, int edx_unused, unsigned int flags)
{
    C3DMeshFileMaterialExtChunk_dtor(chunk);
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}