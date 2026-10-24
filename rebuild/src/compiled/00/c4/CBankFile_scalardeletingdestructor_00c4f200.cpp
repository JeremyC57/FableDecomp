struct CBankFile {
// Leaf: `scalar_deleting_destructor'
    void* vftable;
};

void __fastcall CBankFile_dtor_00c4f220(CBankFile* file);
extern void free_engine(void* p);

void* __fastcall CBankFile_scalar_deleting_destructor(CBankFile* file, int edx_unused, unsigned int flags)
{
    CBankFile_dtor_00c4f220(file);
    if (flags & 1)
        free_engine(file);
    return file;
}