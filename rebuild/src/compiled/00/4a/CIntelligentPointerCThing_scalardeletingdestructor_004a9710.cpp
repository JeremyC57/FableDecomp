struct CIntelligentPointer {
    void* vftable;
};

void __fastcall CIntelligentPointer_dtor(CIntelligentPointer* pointer);
void operator delete(void*);

void* __fastcall CIntelligentPointer_scalar_deleting_destructor(CIntelligentPointer* pointer, int edx_unused, unsigned int flags)
{
    CIntelligentPointer_dtor(pointer);
    if (flags & 1)
        operator delete(pointer);
    return pointer;
}