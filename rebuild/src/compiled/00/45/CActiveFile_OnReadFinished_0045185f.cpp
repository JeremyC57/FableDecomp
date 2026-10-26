#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile {
    void* buf;
};

extern void __fastcall CActiveFile_Notify(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Notify(self);
    void* p = self->buf;
    if (p)
        free(p);
}