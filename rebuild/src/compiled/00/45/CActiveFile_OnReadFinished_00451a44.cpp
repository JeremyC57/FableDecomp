#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile { void* p; };
extern void __fastcall CActiveFile_Notify_004511f3(CActiveFile* self);
extern void __cdecl FreeBuffer(void* p);
void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Notify_004511f3(self);
    void* q = self->p;
    if (q)
        FreeBuffer(q);
}