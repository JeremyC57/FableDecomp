struct CActiveFile {
// Leaf: OnReadFinished
    void* p; // +0x0
    void* q; // +0x4
};

extern "C" void __fastcall CActiveFile_Helper_00440730(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper_00440730(self);
    void* p = self->p;
    if (p)
        free(p);
}