#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile { void* p; };
extern void __fastcall CActiveFile_Notify_004511f3(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Notify_004511f3(self);
    void* q = self->p;
    if (q)
        free(q);
}