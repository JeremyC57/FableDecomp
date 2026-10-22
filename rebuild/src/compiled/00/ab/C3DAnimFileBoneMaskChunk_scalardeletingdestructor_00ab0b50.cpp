struct C3DAnimFileBoneMaskChunk {
    void* vftable;
};

void __fastcall C3DAnimFileBoneMaskChunk_dtor(C3DAnimFileBoneMaskChunk* chunk);
void operator delete(void*);

void* __fastcall C3DAnimFileBoneMaskChunk_scalar_deleting_destructor(C3DAnimFileBoneMaskChunk* chunk, int edx_unused, unsigned int flags)
{
    C3DAnimFileBoneMaskChunk_dtor(chunk);
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}