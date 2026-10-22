#pragma optimize("s",on)
struct CInner;
extern void __fastcall Inner_Process(CInner* self, int edxparam, char* p);
void __cdecl free(void*);

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