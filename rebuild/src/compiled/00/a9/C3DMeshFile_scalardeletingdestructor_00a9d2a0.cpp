class C3DMeshFile { public:
    void* vftable;
};

void __fastcall C3DMeshFile_dtor(C3DMeshFile* file);
void operator delete(void*);

void* __fastcall C3DMeshFile_scalar_deleting_destructor(C3DMeshFile* file, int edx_unused, unsigned int flags)
{
    C3DMeshFile_dtor(file);
    if (flags & 1)
        operator delete(file);
    return file;
}