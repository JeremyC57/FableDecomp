struct CIconText {
    void* vftable;
};

void __fastcall CIconText_dtor(CIconText* text);
void operator delete(void*);

void* __fastcall CIconText_scalar_deleting_destructor(CIconText* text, int edx_unused, unsigned int flags)
{
    CIconText_dtor(text);
    if (flags & 1)
        operator delete(text);
    return text;
}