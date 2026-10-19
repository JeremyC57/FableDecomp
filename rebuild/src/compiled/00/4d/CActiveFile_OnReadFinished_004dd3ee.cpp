#pragma optimize("s",on)
struct CActiveFile {
    void* m_ptr;
};

void __fastcall Helper_004dba86(CActiveFile* self);
void __cdecl free(void*);

void __fastcall OnReadFinished_9(CActiveFile* self)
{
    Helper_004dba86(self);
    void* p = self->m_ptr;
    if (p) {
        free(p);
    }
}