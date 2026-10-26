class CTCBed { public:
    void* vftable;
};

void __fastcall CTCBed_dtor(CTCBed* bed);
void operator delete(void*);

void* __fastcall CTCBed_scalar_deleting_destructor(CTCBed* bed, int edx_unused, unsigned int flags)
{
    CTCBed_dtor(bed);
    if (flags & 1)
        operator delete(bed);
    return bed;
}