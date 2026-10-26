#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile;
extern "C" void __fastcall helper_4db795(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
struct CActiveFile { void* p0; };
extern "C" void __fastcall OnReadFinished_2(CActiveFile* self)
{
    helper_4db795(self);
    void* p = self->p0;
    if (p)
        free(p);
}