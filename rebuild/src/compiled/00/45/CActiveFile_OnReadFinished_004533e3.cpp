#pragma optimize("s",on)
struct CActiveFile {
    void* p;    // +0x0
    void* DstFileHandle;   // +0x4
};

extern void __fastcall Consume_00450ab2(void* ecxArg, void* edxArg, char* buf);
extern void FreeThing(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char local;
    Consume_00450ab2(self->p, self->DstFileHandle, &local);
    void* q = self->p;
    if (q) {
        FreeThing(q);
    }
}