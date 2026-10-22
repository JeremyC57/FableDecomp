struct CVertexBufferBase {
    void* vftable;
};

void __fastcall CVertexBufferBase_dtor(CVertexBufferBase* buffer);
void operator delete(void*);

void* __fastcall CVertexBufferBase_scalar_deleting_destructor(CVertexBufferBase* buffer, int edx_unused, unsigned int flags)
{
    CVertexBufferBase_dtor(buffer);
    if (flags & 1)
        operator delete(buffer);
    return buffer;
}