#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile;
void __fastcall helper_4db88a(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CActiveFile {
    void* field0;
};

void __fastcall OnReadFinished_5(CActiveFile* self)
{
    helper_4db88a(self);
    void* p = self->field0;
    if (p)
        free(p);
}