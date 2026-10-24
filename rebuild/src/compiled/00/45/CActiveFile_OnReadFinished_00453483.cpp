#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile {
    void* p;     // +0x0
    int DstFileHandle;  // +0x4
};

extern int __fastcall CActiveFile_cb(void* ecx, int edx, char* arg);
extern void __cdecl CActiveFile_free(void* q);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char buf;
    CActiveFile_cb(self->p, self->DstFileHandle, &buf);
    void* q = self->p;
    if (q)
        CActiveFile_free(q);
}