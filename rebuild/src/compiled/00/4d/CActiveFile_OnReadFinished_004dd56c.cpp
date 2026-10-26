#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile { void* field0; };

extern void __fastcall CActiveFile_helper_004dbc3a(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall OnReadFinished_13(CActiveFile* self)
{
    CActiveFile_helper_004dbc3a(self);
    void* p = self->field0;
    if (p)
        free(p);
}