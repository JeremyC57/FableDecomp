struct CActiveFile {
// Leaf: OnReadFinished
    void* p; // +0x0
    void* q; // +0x4
};

extern "C" void __fastcall CActiveFile_Helper_0043f230(CActiveFile* self);
extern "C" void __cdecl CActiveFile_Free(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper_0043f230(self);
    void* p = self->p;
    if (p)
        CActiveFile_Free(p);
}