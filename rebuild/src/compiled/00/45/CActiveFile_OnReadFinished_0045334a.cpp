#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CInner;
extern void __fastcall Inner_Process_00450a90(CInner* self, int arg, char* out);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CActiveFile {
    CInner* p;   /* +0x0 */
    int     arg; /* +0x4 */
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char local;
    Inner_Process_00450a90(self->p, self->arg, &local);
    if (self->p != 0)
        free(self->p);
}