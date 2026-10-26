// CActiveFile::OnReadFinished @ 0x004aca50
// Leaf: OnReadFinished
struct CActiveFile {
    void* buffer; // +0
};

// base helper at 0x4ac270 (thiscall, this in ecx)
extern void __fastcall CActiveFile_Base(CActiveFile* self);
// free at 0xbfea14 (cdecl)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Base(self);
    void* p = self->buffer;
    if (p)
        free(p);
}