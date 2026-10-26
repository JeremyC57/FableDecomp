class CTCDivineWrath { public:
    void* vftable;
};

void __fastcall CTCDivineWrath_dtor(CTCDivineWrath* wrath);
void operator delete(void*);

void* __fastcall CTCDivineWrath_scalar_deleting_destructor(CTCDivineWrath* wrath, int edx_unused, unsigned int flags)
{
    CTCDivineWrath_dtor(wrath);
    if (flags & 1)
        operator delete(wrath);
    return wrath;
}