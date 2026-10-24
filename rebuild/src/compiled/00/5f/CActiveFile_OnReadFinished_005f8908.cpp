#pragma optimize("s",on)
// Leaf: OnReadFinished

struct CActiveFile {
    void* buf;
};

extern "C" void __fastcall CActiveFile_PreRead_005f8197(CActiveFile* self);
extern "C" void __cdecl retail_free(void* p);

void __fastcall OnReadFinished(CActiveFile* self)
{
    CActiveFile_PreRead_005f8197(self);
    void* p = self->buf;
    if (p)
        retail_free(p);
}