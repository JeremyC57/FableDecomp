#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile {
    void* p; // +0x0
    void* q; // +0x4
};

extern void __fastcall Reader_Read(void* ecx, void* edx, unsigned char* buf);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    unsigned char b;
    Reader_Read(self->p, self->q, &b);
    if (self->p) {
        free(self->p);
    }
}