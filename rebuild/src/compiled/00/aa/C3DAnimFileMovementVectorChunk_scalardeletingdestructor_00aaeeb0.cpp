class C3DAnimFileMovementVectorChunk { public:
    void* vftable;
};

void __fastcall C3DAnimFileMovementVectorChunk_dtor(C3DAnimFileMovementVectorChunk* chunk);
void operator delete(void*);

void* __fastcall C3DAnimFileMovementVectorChunk_scalar_deleting_destructor(C3DAnimFileMovementVectorChunk* chunk, int edx_unused, unsigned int flags)
{
    C3DAnimFileMovementVectorChunk_dtor(chunk);
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}