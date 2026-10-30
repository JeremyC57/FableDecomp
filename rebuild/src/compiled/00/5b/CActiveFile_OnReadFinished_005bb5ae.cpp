#pragma optimize("s",on)
// Leaf: OnReadFinished
extern "C" void __fastcall CActiveFile_OnReadHelper(void* self);
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)
struct CActiveFile { void* buf; };
void __fastcall OnReadFinished(CActiveFile* self)
{
    CActiveFile_OnReadHelper(self);
    void* p = self->buf;
    if (p)
        free(p);
}