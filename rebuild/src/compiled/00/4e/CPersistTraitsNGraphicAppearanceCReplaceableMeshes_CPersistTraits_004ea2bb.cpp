extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
// Leaf: OnReadFinished
struct S { void* p; };
void __fastcall OnReadFinished(S* self)
{
    if (self->p)
        free(self->p);
}