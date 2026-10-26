class CInputProcessPhotojournalCapture { public:
    void* vftable;
};

void __fastcall CInputProcessPhotojournalCapture_dtor(CInputProcessPhotojournalCapture* process);
void operator delete(void*);

void* __fastcall CInputProcessPhotojournalCapture_scalar_deleting_destructor(CInputProcessPhotojournalCapture* process, int edx_unused, unsigned int flags)
{
    CInputProcessPhotojournalCapture_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}