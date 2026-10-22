struct CMovie {
    void* vftable;
};

void __fastcall CMovie_dtor(CMovie* movie);
void operator delete(void*);

void* __fastcall CMovie_scalar_deleting_destructor(CMovie* movie, int edx_unused, unsigned int flags)
{
    CMovie_dtor(movie);
    if (flags & 1)
        operator delete(movie);
    return movie;
}