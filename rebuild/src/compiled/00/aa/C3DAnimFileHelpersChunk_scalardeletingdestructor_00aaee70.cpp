class C3DAnimFileHelpersChunk { public:
    void* vftable;
};

void __fastcall C3DAnimFileHelpersChunk_dtor(C3DAnimFileHelpersChunk* chunk);
void operator delete(void*);

void* __fastcall C3DAnimFileHelpersChunk_scalar_deleting_destructor(C3DAnimFileHelpersChunk* chunk, int edx_unused, unsigned int flags)
{
    C3DAnimFileHelpersChunk_dtor(chunk);
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}