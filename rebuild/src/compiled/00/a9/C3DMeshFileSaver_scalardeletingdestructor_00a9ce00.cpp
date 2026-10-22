struct C3DMeshFileSaver {
    void* vftable;
};

void __fastcall C3DMeshFileSaver_dtor(C3DMeshFileSaver* saver);
void operator delete(void*);

void* __fastcall C3DMeshFileSaver_scalar_deleting_destructor(C3DMeshFileSaver* saver, int edx_unused, unsigned int flags)
{
    C3DMeshFileSaver_dtor(saver);
    if (flags & 1)
        operator delete(saver);
    return saver;
}