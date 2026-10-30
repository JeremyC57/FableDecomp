// CActiveFile::OnReadFinished @ 00835390
// Leaf: OnReadFinished
// __fastcall void OnReadFinished(CActiveFile* this)

void __fastcall CActiveFile_BaseOnReadFinished(void* self);  // 0x834f90
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

struct CActiveFile {
    void* buffer;  // +0x00
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_BaseOnReadFinished(self);
    void* p = self->buffer;
    if (p)
        free(p);
}