class C3DMeshFileXMappingInfoChunk { public:
    void* vftable;
};

void __fastcall C3DMeshFileXMappingInfoChunk_dtor(C3DMeshFileXMappingInfoChunk* chunk);
void operator delete(void*);

void* __fastcall C3DMeshFileXMappingInfoChunk_scalar_deleting_destructor(C3DMeshFileXMappingInfoChunk* chunk, int edx_unused, unsigned int flags)
{
    C3DMeshFileXMappingInfoChunk_dtor(chunk);
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}