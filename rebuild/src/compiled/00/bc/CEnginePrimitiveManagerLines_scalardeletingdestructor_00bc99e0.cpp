struct CEnginePrimitiveManagerLines {
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerLines_dtor(CEnginePrimitiveManagerLines* lines);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerLines_scalar_deleting_destructor(CEnginePrimitiveManagerLines* lines, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerLines_dtor(lines);
    if (flags & 1)
        operator delete(lines);
    return lines;
}