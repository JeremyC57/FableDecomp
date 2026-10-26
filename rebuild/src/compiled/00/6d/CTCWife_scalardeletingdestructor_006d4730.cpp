class CTCWife { public:
    void* vftable;
};

void __fastcall CTCWife_dtor(CTCWife* wife);
void operator delete(void*);

void* __fastcall CTCWife_scalar_deleting_destructor(CTCWife* wife, int edx_unused, unsigned int flags)
{
    CTCWife_dtor(wife);
    if (flags & 1)
        operator delete(wife);
    return wife;
}