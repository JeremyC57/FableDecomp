#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile {
    void* p;   // +0x0
    void* q;   // +0x4
};

// first callee: __fastcall(ecx, edx, stack &byte)
extern void __fastcall Consume(void* a, void* b, char* out);
// second callee: __cdecl free-style, caller pops 1 arg
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char status;
    Consume(self->p, self->q, &status);
    if (self->p)
        free(self->p);
}