struct C3DMeshFileXAnimatedPrimitiveBlockChunk {
    void* vftable;
};

void __fastcall C3DMeshFileXAnimatedPrimitiveBlockChunk_dtor(C3DMeshFileXAnimatedPrimitiveBlockChunk* chunk);
void operator delete(void*);

void* __fastcall C3DMeshFileXAnimatedPrimitiveBlockChunk_scalar_deleting_destructor(C3DMeshFileXAnimatedPrimitiveBlockChunk* chunk, int edx_unused, unsigned int flags)
{
    C3DMeshFileXAnimatedPrimitiveBlockChunk_dtor(chunk);
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}