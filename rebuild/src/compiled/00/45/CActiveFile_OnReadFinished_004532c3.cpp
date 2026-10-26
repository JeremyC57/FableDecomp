#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile {
    void* p0; // +0x0
    void* p1; // +0x4
};

extern void __fastcall Handler(void* a, void* b, char* c);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char local;
    Handler(self->p0, self->p1, &local);
    if (self->p0)
        free(self->p0);
}