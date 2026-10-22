struct CTCWaterWader {
    void* vftable;
};

void __fastcall CTCWaterWader_dtor(CTCWaterWader* wader);
void operator delete(void*);

void* __fastcall CTCWaterWader_scalar_deleting_destructor(CTCWaterWader* wader, int edx_unused, unsigned int flags)
{
    CTCWaterWader_dtor(wader);
    if (flags & 1)
        operator delete(wader);
    return wader;
}