class C3DMeshFileXBoundingSphereChunk { public:
    void* vftable;
};

void __fastcall C3DMeshFileXBoundingSphereChunk_dtor(C3DMeshFileXBoundingSphereChunk* chunk);
void operator delete(void*);

void* __fastcall C3DMeshFileXBoundingSphereChunk_scalar_deleting_destructor(C3DMeshFileXBoundingSphereChunk* chunk, int edx_unused, unsigned int flags)
{
    C3DMeshFileXBoundingSphereChunk_dtor(chunk);
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}