#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile {
    void* p;     // +0x0
    int DstFileHandle;  // +0x4
};

extern int __fastcall CActiveFile_cb(void* ecx, int edx, char* arg);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char buf;
    CActiveFile_cb(self->p, self->DstFileHandle, &buf);
    void* q = self->p;
    if (q)
        free(q);
}