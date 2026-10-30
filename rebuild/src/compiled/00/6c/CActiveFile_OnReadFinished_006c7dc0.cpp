// CActiveFile::OnReadFinished @ 006c7dc0
// Leaf: OnReadFinished
struct CActiveFile {
    void* m_thing;   // +0x00
};

void __fastcall CActiveFile_Base_006c7670(CActiveFile* self);   // 0x6c7670
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

void __fastcall CActiveFile__OnReadFinished(CActiveFile* self)
{
    CActiveFile_Base_006c7670(self);
    void* p = self->m_thing;
    if (p != 0)
        free(p);
}