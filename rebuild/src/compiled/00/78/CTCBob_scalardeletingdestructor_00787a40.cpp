class CTCBob { public:
    void* vftable;
};

void __fastcall CTCBob_dtor(CTCBob* bob);
void operator delete(void*);

void* __fastcall CTCBob_scalar_deleting_destructor(CTCBob* bob, int edx_unused, unsigned int flags)
{
    CTCBob_dtor(bob);
    if (flags & 1)
        operator delete(bob);
    return bob;
}