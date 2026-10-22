struct CInkeyBuffer {
    void* vftable;
};

void __fastcall CInkeyBuffer_dtor(CInkeyBuffer* buffer);
void operator delete(void*);

void* __fastcall CInkeyBuffer_scalar_deleting_destructor(CInkeyBuffer* buffer, int edx_unused, unsigned int flags)
{
    CInkeyBuffer_dtor(buffer);
    if (flags & 1)
        operator delete(buffer);
    return buffer;
}