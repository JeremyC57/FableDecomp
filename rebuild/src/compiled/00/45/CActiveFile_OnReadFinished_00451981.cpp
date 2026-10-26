#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile;
extern "C" void __fastcall CActiveFile_helper(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CActiveFile { void* buf; };

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_helper(self);
    void* p = self->buf;
    if (p)
        free(p);
}