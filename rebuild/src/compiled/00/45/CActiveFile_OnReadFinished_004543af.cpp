#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile {
    void* p; // +0x0
};

extern "C" void __fastcall CActiveFile_Helper_00453607(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper_00453607(self);
    void* q = self->p;
    if (q != 0)
        free(q);
}