struct CActiveFile {
// Leaf: OnReadFinished
    void* p;
    void* q;
};
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    void* h = self->p;
    if (h)
        free(h);
}