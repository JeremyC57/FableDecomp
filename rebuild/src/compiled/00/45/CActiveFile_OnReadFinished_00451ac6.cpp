#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile { void* buf; };
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
void __fastcall CActiveFile_helper(CActiveFile* self);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_helper(self);
    void* p = self->buf;
    if (p)
        free(p);
}