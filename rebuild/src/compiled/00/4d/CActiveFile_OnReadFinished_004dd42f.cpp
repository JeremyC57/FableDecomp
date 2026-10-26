#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile { void* field0; };

void __fastcall Helper_4dbad3(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall OnReadFinished_10(CActiveFile* self)
{
    Helper_4dbad3(self);
    void* p = self->field0;
    if (p)
        free(p);
}