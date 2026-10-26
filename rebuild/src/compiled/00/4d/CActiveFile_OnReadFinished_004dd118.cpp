#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile;
void __fastcall Helper_4db748(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
struct CActiveFile { void* field0; };
void __fastcall OnReadFinished(CActiveFile* self)
{
    Helper_4db748(self);
    void* p = self->field0;
    if (p)
        free(p);
}