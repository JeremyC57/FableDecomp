// CActiveFile::OnReadFinished @ 0x0070ba50
// Leaf: OnReadFinished
struct CActiveFile { void* ptr; };

extern void __fastcall base_OnReadFinished_impl(CActiveFile* self);
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

void __fastcall OnReadFinished(CActiveFile* self)
{
    base_OnReadFinished_impl(self);
    void* p = self->ptr;
    if (p)
        free(p);
}