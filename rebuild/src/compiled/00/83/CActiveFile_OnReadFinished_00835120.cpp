// CActiveFile::OnReadFinished  @ 0x00835120  __fastcall(void)
// Leaf: OnReadFinished

struct CActiveFile;

void __fastcall CActiveFile_sub_834F90(CActiveFile* self);  // 0x00834F90 __fastcall
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

struct CActiveFile
{
    void* m_ptr;   // +0x00
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_sub_834F90(self);
    void* p = self->m_ptr;
    if (p)
        free(p);
}