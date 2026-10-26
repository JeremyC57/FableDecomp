class CTCDrunkenness { public:
    void* vftable;
};

void __fastcall CTCDrunkenness_dtor(CTCDrunkenness* drunkenness);
void operator delete(void*);

void* __fastcall CTCDrunkenness_scalar_deleting_destructor(CTCDrunkenness* drunkenness, int edx_unused, unsigned int flags)
{
    CTCDrunkenness_dtor(drunkenness);
    if (flags & 1)
        operator delete(drunkenness);
    return drunkenness;
}