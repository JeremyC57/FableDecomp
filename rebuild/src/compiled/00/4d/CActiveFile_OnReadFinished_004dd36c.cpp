#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile { void* m0; };

// 0x4db9ec - this-in-ecx cleanup on the CActiveFile
extern void __fastcall CAF_cleanup(CActiveFile* self);
// 0xbfea14 - cdecl free/operator delete (single stack arg, caller pops)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall OnReadFinished_7(CActiveFile* self)
{
    CAF_cleanup(self);
    void* p = self->m0;
    if (p)
        free(p);
}