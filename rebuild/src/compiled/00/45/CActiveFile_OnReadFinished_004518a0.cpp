#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile { void* buf; void Close(); };
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    self->Close();
    void* p = self->buf;
    if (p != 0)
        free(p);
}