class CTCHeroCentre { public:
    void* vftable;
};

void __fastcall CTCHeroCentre_dtor(CTCHeroCentre* centre);
void operator delete(void*);

void* __fastcall CTCHeroCentre_scalar_deleting_destructor(CTCHeroCentre* centre, int edx_unused, unsigned int flags)
{
    CTCHeroCentre_dtor(centre);
    if (flags & 1)
        operator delete(centre);
    return centre;
}