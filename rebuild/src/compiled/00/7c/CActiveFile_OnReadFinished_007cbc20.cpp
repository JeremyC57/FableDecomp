// CActiveFile::OnReadFinished @ 007cbc20
// Leaf: OnReadFinished
struct CActiveFile {
    void* m_ptr;
};

// __fastcall base call at 0x7cbb80
void __fastcall OnReadFinishedBase(CActiveFile* self);
// cdecl free function at 0xbfea14
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    OnReadFinishedBase(self);
    void* p = self->m_ptr;
    if (p)
        free(p);
}