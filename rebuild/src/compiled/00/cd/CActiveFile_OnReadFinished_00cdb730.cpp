// CActiveFile::OnReadFinished  @ 00cdb730

void __fastcall CActiveFile_Helper_00cd4df0(void* self);   // 0xcd4df0  (__fastcall on self)
void CActiveFile_Free(void* p);                    // 0xbfea14  (cdecl free)

struct CActiveFile
{
    void* m_buffer;   // +0x00

    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    CActiveFile_Helper_00cd4df0(this);
    void* p = this->m_buffer;
    if (p)
        CActiveFile_Free(p);
}