#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile { void* p; };
extern "C" void __fastcall CActiveFile_BaseNotify(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_BaseNotify(self);
    void* buf = self->p;
    if (buf)
        free(buf);
}