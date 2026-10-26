class CMemoryFile { public:
    void* vptr;      // +0x0
    char pad4[0x0C]; // +0x4 .. +0x10
    void* member10;  // +0x10
    char pad14[0x0C];// +0x14 .. +0x20
    char Pathname; // +0x20 CWideString (destructed via ecx=esi+0x20; retail STLport layout: CArray 0xc puts Opened at +0x1c)
};

extern const void* CMemoryFile_vtbl[];
extern void __fastcall Cleanup_5d72d0(void* file);
extern void __fastcall SubDtor_589450(void* sub);
extern void __cdecl Free_7ec954(void* p);
extern void __fastcall BaseDtor_588240(void* file);
extern void __cdecl OpDelete_7ec8fc(void* p);

void* __fastcall CMemoryFile_scalar_deleting_destructor(CMemoryFile* file, void* /*edx*/, unsigned int flags)
{
    file->vptr = (void*)CMemoryFile_vtbl;
    Cleanup_5d72d0(file);
    SubDtor_589450(&file->Pathname);
    if (file->member10)
        Free_7ec954(file->member10);
    BaseDtor_588240(file);
    if (flags & 1)
        OpDelete_7ec8fc(file);
    return file;
}