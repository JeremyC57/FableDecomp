struct CActiveFile {
// Leaf: OnReadFinished
    void* p; // +0x0
    void* q; // +0x4
};

extern void __fastcall CActiveFile_Base_OnReadFinished(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Base_OnReadFinished(self);
    void* buf = self->p;
    if (buf)
        free(buf);
}