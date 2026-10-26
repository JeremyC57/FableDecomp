#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile;
extern void __fastcall CActiveFile_helper_004dba39(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
struct CActiveFile { void* m_ptr; };
void __fastcall OnReadFinished_8(CActiveFile* self)
{
    CActiveFile_helper_004dba39(self);
    void* p = self->m_ptr;
    if (p)
        free(p);
}