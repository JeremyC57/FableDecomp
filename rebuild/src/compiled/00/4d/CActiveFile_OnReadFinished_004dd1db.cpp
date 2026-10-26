#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile { void* p0; };

void __fastcall helper_4db853(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall OnReadFinished_4(CActiveFile* self)
{
    helper_4db853(self);
    void* p = self->p0;
    if (p)
        free(p);
}