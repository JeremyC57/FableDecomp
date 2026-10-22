struct CMapFilenames {
    void* vftable;
};

void __fastcall CMapFilenames_dtor(CMapFilenames* filenames);
void operator delete(void*);

void* __fastcall CMapFilenames_scalar_deleting_destructor(CMapFilenames* filenames, int edx_unused, unsigned int flags)
{
    CMapFilenames_dtor(filenames);
    if (flags & 1)
        operator delete(filenames);
    return filenames;
}