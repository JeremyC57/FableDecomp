#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile {
    void* buf;
};

void __fastcall CActiveFile_Helper_00485aef(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper_00485aef(self);
    void* p = self->buf;
    if (p)
        free(p);
}