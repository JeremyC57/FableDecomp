// CActiveFile::OnReadFinished @ 0x00743b10
// Leaf: OnReadFinished
struct CActiveFile {
    void* member0;
};

extern "C" void __fastcall CActiveFile_base_00742290(CActiveFile* self); // 0x742290
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_base_00742290(self);
    void* m = self->member0;
    if (m) {
        free(m);
    }
}