#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile { void* field0; };
extern void __fastcall CActiveFile_step(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_step(self);
    void* p = self->field0;
    if (p) {
        free(p);
    }
}