#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile {
    void* m_ptr;
};

void __fastcall Helper_004dba86(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall OnReadFinished_9(CActiveFile* self)
{
    Helper_004dba86(self);
    void* p = self->m_ptr;
    if (p) {
        free(p);
    }
}