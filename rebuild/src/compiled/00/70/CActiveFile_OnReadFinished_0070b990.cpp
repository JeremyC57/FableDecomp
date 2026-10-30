// CActiveFile::OnReadFinished @ 0070b990
// Leaf: OnReadFinished
// base_70b8a0(this); p = *this; if(p) free_bfea14(p);

struct CActiveFile;

extern "C" void __fastcall base_70b8a0(CActiveFile* self);
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

struct CActiveFile {
    void* ptr; // +0
};

void __fastcall OnReadFinished(CActiveFile* self)
{
    base_70b8a0(self);
    void* p = *(void**)self;
    if (p)
        free(p);
}