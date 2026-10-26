#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile { CActiveFile* f0; };

// thiscall helper (this in ecx)
extern void __fastcall sub_4dbb20(CActiveFile* self);
// cdecl free function taking pointer on stack
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall OnReadFinished_11(CActiveFile* self)
{
    sub_4dbb20(self);
    CActiveFile* p = self->f0;
    if (p)
        free(p);
}