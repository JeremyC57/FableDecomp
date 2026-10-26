class CInputProcessBoastUI { public:
    void* vftable;
};

void __fastcall CInputProcessBoastUI_dtor(CInputProcessBoastUI* process);
void operator delete(void*);

void* __fastcall CInputProcessBoastUI_scalar_deleting_destructor(CInputProcessBoastUI* process, int edx_unused, unsigned int flags)
{
    CInputProcessBoastUI_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}