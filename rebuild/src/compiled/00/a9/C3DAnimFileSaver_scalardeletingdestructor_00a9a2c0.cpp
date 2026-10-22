struct C3DAnimFileSaver {
    void* vftable;
};

void __fastcall C3DAnimFileSaver_dtor(C3DAnimFileSaver* saver);
void operator delete(void*);

void* __fastcall C3DAnimFileSaver_scalar_deleting_destructor(C3DAnimFileSaver* saver, int edx_unused, unsigned int flags)
{
    C3DAnimFileSaver_dtor(saver);
    if (flags & 1)
        operator delete(saver);
    return saver;
}