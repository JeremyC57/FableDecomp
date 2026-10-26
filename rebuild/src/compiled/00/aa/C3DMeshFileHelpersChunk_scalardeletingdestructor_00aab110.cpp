class C3DMeshFileHelpersChunk { public:
    void* vftable;
};

void __fastcall C3DMeshFileHelpersChunk_dtor(C3DMeshFileHelpersChunk* chunk);
void operator delete(void*);

void* __fastcall C3DMeshFileHelpersChunk_scalar_deleting_destructor(C3DMeshFileHelpersChunk* chunk, int edx_unused, unsigned int flags)
{
    C3DMeshFileHelpersChunk_dtor(chunk);
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}