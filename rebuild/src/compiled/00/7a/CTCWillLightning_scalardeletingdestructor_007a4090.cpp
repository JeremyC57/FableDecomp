class CTCWillLightning { public:
    void* vftable;
};

void __fastcall CTCWillLightning_dtor(CTCWillLightning* lightning);
void operator delete(void*);

void* __fastcall CTCWillLightning_scalar_deleting_destructor(CTCWillLightning* lightning, int edx_unused, unsigned int flags)
{
    CTCWillLightning_dtor(lightning);
    if (flags & 1)
        operator delete(lightning);
    return lightning;
}