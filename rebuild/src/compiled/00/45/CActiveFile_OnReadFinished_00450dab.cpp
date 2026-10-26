#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile {
    void* p; // +0x0
    void* q; // +0x4
};

extern void __fastcall CActiveFile_Helper(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper(self);
    void* b = self->p;
    if (b)
        free(b);
}