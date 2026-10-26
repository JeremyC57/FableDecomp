#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile;
extern "C" void __fastcall AF_helper(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
struct CActiveFile { void* field0; };
extern "C" void __fastcall OnReadFinished_12(CActiveFile* self)
{
    AF_helper(self);
    void* p = self->field0;
    if (p)
        free(p);
}