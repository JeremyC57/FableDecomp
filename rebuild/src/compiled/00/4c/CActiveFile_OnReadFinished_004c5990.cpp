// CActiveFile::OnReadFinished @ 004c5990
// Leaf: OnReadFinished
struct CActiveFile {
    void* buf;   // +0x00
};

// base method called with ecx=this (__fastcall modeled as __fastcall)
void __fastcall CActiveFile_base_004c5690(CActiveFile* self);
// cdecl free-like function
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_base_004c5690(self);
    void* p = self->buf;
    if (p)
        free(p);
}