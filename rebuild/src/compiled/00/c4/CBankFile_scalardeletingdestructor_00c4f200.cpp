struct CBankFile {
    void* vftable;
};

void __fastcall CBankFile_dtor(CBankFile* file);
extern void free_engine(void* p);

void* __fastcall CBankFile_scalar_deleting_destructor(CBankFile* file, int edx_unused, unsigned int flags)
{
    CBankFile_dtor(file);
    if (flags & 1)
        free_engine(file);
    return file;
}