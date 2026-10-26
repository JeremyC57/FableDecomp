class CWADFile { public:
    void* vftable;
};

void __fastcall CWADFile_dtor(CWADFile* file);
void operator delete(void*);

void* __fastcall CWADFile_scalar_deleting_destructor(CWADFile* file, int edx_unused, unsigned int flags)
{
    CWADFile_dtor(file);
    if (flags & 1)
        operator delete(file);
    return file;
}