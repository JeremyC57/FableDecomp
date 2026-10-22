#pragma optimize("s",on)
struct CActiveFile {
    void* p;    // +0x0
    void* DstFileHandle;   // +0x4
};

extern void __fastcall Consume(void* ecxArg, void* edxArg, char* buf);
extern void FreeThing(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char local;
    Consume(self->p, self->DstFileHandle, &local);
    void* q = self->p;
    if (q) {
        FreeThing(q);
    }
}