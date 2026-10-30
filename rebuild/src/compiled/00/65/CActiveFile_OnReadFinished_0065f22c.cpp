#pragma optimize("s",on)
// Leaf: OnReadFinished

struct CActiveFile {
    void* buf;
};

extern "C" void __fastcall CActiveFile_PreRead_0065e535(CActiveFile* self);
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

void __fastcall OnReadFinished(CActiveFile* self)
{
    CActiveFile_PreRead_0065e535(self);
    void* p = self->buf;
    if (p)
        free(p);
}