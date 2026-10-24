#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile {
    void* p;    // +0x0
    void* DstFileHandle;   // +0x4
};

extern void __fastcall Reader_Op(void* self, void* edx, char* buf);
extern void FreeThing(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char local;
    Reader_Op(self->p, self->DstFileHandle, &local);
    if (self->p) {
        FreeThing(self->p);
    }
}