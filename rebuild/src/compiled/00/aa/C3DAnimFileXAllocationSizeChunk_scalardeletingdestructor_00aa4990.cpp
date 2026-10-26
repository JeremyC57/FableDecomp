class C3DAnimFileXAllocationSizeChunk { public:
    void* vftable;
};

void __fastcall C3DAnimFileXAllocationSizeChunk_dtor(C3DAnimFileXAllocationSizeChunk* chunk);
void operator delete(void*);

void* __fastcall C3DAnimFileXAllocationSizeChunk_scalar_deleting_destructor(C3DAnimFileXAllocationSizeChunk* chunk, int edx_unused, unsigned int flags)
{
    C3DAnimFileXAllocationSizeChunk_dtor(chunk);
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}