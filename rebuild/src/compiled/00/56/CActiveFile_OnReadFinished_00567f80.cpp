// CActiveFile::OnReadFinished @ 0x00567f80
// Leaf: OnReadFinished
struct CActiveFile { void* m0; };

extern void __fastcall CActiveFile_FrameUpdate_566dc0(CActiveFile* self); // thiscall this->ecx, void
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_FrameUpdate_566dc0(self);
    void* p = self->m0;
    if (p)
        free(p);
}