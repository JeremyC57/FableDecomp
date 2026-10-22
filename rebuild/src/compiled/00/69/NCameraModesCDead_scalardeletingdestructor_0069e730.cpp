struct CDead {
    void* vftable;
};

void __fastcall CDead_dtor(CDead* dead);
void operator delete(void*);

void* __fastcall CDead_scalar_deleting_destructor(CDead* dead, int edx_unused, unsigned int flags)
{
    CDead_dtor(dead);
    if (flags & 1)
        operator delete(dead);
    return dead;
}