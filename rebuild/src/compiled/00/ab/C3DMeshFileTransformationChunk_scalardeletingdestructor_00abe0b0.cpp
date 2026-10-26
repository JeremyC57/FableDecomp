class C3DMeshFileTransformationChunk { public:
    void* vftable;
};

void __fastcall C3DMeshFileTransformationChunk_dtor(C3DMeshFileTransformationChunk* chunk);
void operator delete(void*);

void* __fastcall C3DMeshFileTransformationChunk_scalar_deleting_destructor(C3DMeshFileTransformationChunk* chunk, int edx_unused, unsigned int flags)
{
    C3DMeshFileTransformationChunk_dtor(chunk);
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}