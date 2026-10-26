struct CInner;
void __fastcall CConsoleInputBase_dtor_inner(CInner* input);

class CConsoleInputBase { public:
    unsigned int vftable;
    char inner[0xb4];
    void* tex;
};

void* __fastcall CConsoleInputBase_scalar_deleting_destructor(CConsoleInputBase* input, void* edx, unsigned int flags)
{
    (void)edx;
    input->vftable = 0x122e5b0;
    CConsoleInputBase_dtor_inner((CInner*)(&input->inner));
    if (flags & 1)
        operator delete(input);
    return input;
}