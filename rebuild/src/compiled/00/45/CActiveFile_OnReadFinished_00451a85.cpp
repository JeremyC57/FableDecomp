#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile { void* payload; };
void __fastcall CActiveFile_Prep(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Prep(self);
    void* p = self->payload;
    if (p)
        free(p);
}