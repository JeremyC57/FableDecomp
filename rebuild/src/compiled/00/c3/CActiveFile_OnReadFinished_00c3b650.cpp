// CActiveFile::OnReadFinished @ 0x00c3b650
// void __fastcall CActiveFile::OnReadFinished(CActiveFile* this)

void operator delete[](void*);

struct CActiveFile {
    void* m_buffer; // +0x00
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    if (this->m_buffer != 0) {
        operator delete[](this->m_buffer);
        this->m_buffer = 0;
    }
}