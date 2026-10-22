struct C3DMeshFileXTextureIndexChunk {
    void* vftable;
};

void __fastcall C3DMeshFileXTextureIndexChunk_dtor(C3DMeshFileXTextureIndexChunk* chunk);
void operator delete(void*);

void* __fastcall C3DMeshFileXTextureIndexChunk_scalar_deleting_destructor(C3DMeshFileXTextureIndexChunk* chunk, int edx_unused, unsigned int flags)
{
    C3DMeshFileXTextureIndexChunk_dtor(chunk);
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}