#pragma optimize("s",on)
// Leaf: OnReadFinished

struct CActiveFile {
    void* buf;
};

void __fastcall CActiveFile_DoFinish(CActiveFile* self);
extern "C" void FreeThing(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_DoFinish(self);
    void* p = self->buf;
    if (p)
        FreeThing(p);
}