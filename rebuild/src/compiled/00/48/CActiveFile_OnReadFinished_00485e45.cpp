#pragma optimize("s",on)
struct CActiveFile {
    void* buf;
};

void __fastcall CActiveFile_Helper_00485aef(CActiveFile* self);
void CActiveFile_Free(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper_00485aef(self);
    void* p = self->buf;
    if (p)
        CActiveFile_Free(p);
}