// CActiveFile::OnReadFinished @ 0x004b8940
// Leaf: OnReadFinished
// __fastcall void OnReadFinished(CActiveFile* this)

struct CActiveFile {
    void* field0;
};

// base handler at 0x4b8050 (same OnReadFinished shape, base)
extern "C" void __fastcall CActiveFile_base(CActiveFile* self);

// operator delete / free at 0xbfea14
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_base(self);
    void* p = self->field0;
    if (p)
        free(p);
}