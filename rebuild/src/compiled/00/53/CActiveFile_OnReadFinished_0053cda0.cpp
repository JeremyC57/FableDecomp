#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile {
    void* p;   // +0x0
    void* q;   // +0x4
};

// first callee: __fastcall(ecx, edx, stack &byte)
extern void __fastcall Consume_0053cc6a(void* a, void* b, char* out);
// second callee: __cdecl free-style, caller pops 1 arg
extern void __cdecl Release(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char status;
    Consume_0053cc6a(self->p, self->q, &status);
    if (self->p)
        Release(self->p);
}