// CActiveFile::OnReadFinished @ 0070ba70
// Leaf: OnReadFinished
struct CActiveFile;

extern "C" void __fastcall CActiveFile_base_0x70ba00(CActiveFile* self);
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

struct CActiveFile {
    void* m_ptr; // offset 0
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_base_0x70ba00(self);
    void* p = self->m_ptr;
    if (p != 0)
        free(p);
}