class CInputProcessCreditsUI { public:
    void* vftable;
};

void __fastcall CInputProcessCreditsUI_dtor(CInputProcessCreditsUI* process);
void operator delete(void*);

void* __fastcall CInputProcessCreditsUI_scalar_deleting_destructor(CInputProcessCreditsUI* process, int edx_unused, unsigned int flags)
{
    CInputProcessCreditsUI_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}