#pragma optimize("s",on)
// Leaf: OnReadFinished
extern "C" void __fastcall CActiveFile_Helper_005bac05(void* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
struct CActiveFile { void* buf; };
void __fastcall OnReadFinished_4(CActiveFile* self)
{
    CActiveFile_Helper_005bac05(self);
    void* p = self->buf;
    if (p)
        free(p);
}