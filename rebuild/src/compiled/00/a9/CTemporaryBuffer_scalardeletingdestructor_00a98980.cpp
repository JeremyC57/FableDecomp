struct CTemporaryBuffer {
    void* vftable;
};

void __fastcall CTemporaryBuffer_dtor(CTemporaryBuffer* buffer);
void operator delete(void*);

void* __fastcall CTemporaryBuffer_scalar_deleting_destructor(CTemporaryBuffer* buffer, int edx_unused, unsigned int flags)
{
    CTemporaryBuffer_dtor(buffer);
    if (flags & 1)
        operator delete(buffer);
    return buffer;
}