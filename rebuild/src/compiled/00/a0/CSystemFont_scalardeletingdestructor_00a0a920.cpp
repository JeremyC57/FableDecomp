struct CSystemFont {
    void* vftable;
};

void __fastcall CSystemFont_dtor(CSystemFont* font);
void operator delete(void*);

void* __fastcall CSystemFont_scalar_deleting_destructor(CSystemFont* font, int edx_unused, unsigned int flags)
{
    CSystemFont_dtor(font);
    if (flags & 1)
        operator delete(font);
    return font;
}