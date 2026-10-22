struct CEditBox {
    void* vftable;
};

void __fastcall CEditBox_dtor(CEditBox* box);
void operator delete(void*);

void* __fastcall CEditBox_scalar_deleting_destructor(CEditBox* box, int edx_unused, unsigned int flags)
{
    CEditBox_dtor(box);
    if (flags & 1)
        operator delete(box);
    return box;
}