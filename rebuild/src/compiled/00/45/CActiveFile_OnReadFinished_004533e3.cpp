#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile {
    void* p;    // +0x0
    void* DstFileHandle;   // +0x4
};

extern void __fastcall Consume_00450ab2(void* ecxArg, void* edxArg, char* buf);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char local;
    Consume_00450ab2(self->p, self->DstFileHandle, &local);
    void* q = self->p;
    if (q) {
        free(q);
    }
}