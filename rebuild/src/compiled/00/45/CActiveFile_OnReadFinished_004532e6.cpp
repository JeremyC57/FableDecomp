#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CInner;
extern void __fastcall Inner_Process(CInner* self, int edxparam, char* p);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CActiveFile {
    CInner* p;   /* +0x0 */
    int     DstFileHandle;  /* +0x4 */
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char local;
    Inner_Process(self->p, self->DstFileHandle, &local);
    CInner* q = self->p;
    if (q)
        free(q);
}