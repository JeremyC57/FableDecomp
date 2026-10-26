class CEnginePrimitiveManagerText { public:
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerText_dtor(CEnginePrimitiveManagerText* text);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerText_scalar_deleting_destructor(CEnginePrimitiveManagerText* text, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerText_dtor(text);
    if (flags & 1)
        operator delete(text);
    return text;
}