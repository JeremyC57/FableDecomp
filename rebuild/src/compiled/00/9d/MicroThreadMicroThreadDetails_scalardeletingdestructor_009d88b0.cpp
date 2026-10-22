struct MicroThreadDetails {
    void* vftable;
};

void __fastcall MicroThreadDetails_dtor(MicroThreadDetails* details);
void operator delete(void*);

void* __fastcall MicroThreadDetails_scalar_deleting_destructor(MicroThreadDetails* details, int edx_unused, unsigned int flags)
{
    MicroThreadDetails_dtor(details);
    if (flags & 1)
        operator delete(details);
    return details;
}