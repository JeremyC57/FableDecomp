class CBankFile { public:
// Leaf: `scalar_deleting_destructor'
    void* vftable;
};

void __fastcall CBankFile_dtor(CBankFile* file);
void operator delete(void*);

void* __fastcall CBankFile_scalar_deleting_destructor(CBankFile* file, int edx_unused, unsigned int flags)
{
    CBankFile_dtor(file);
    if (flags & 1)
        operator delete(file);
    return file;
}