// CLHFile::GetSize(unsigned long* size) -- when the counted file data (+4) exists and is open (+0x19) write its size (+0x10) to the out-pointer and return true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADD160, length 48 bytes.
// Vtable 0x0129F094 slot 0 (labelled vfunc_0). No callees. Shape: the data object in eax, the open byte in cl, `mov al,1` / `xor al,al`, `ret 4`.
struct SFileData_00add160 {                           // the counted file data object
    unsigned char _pad[4];
    long          m_mode;                          // +0x04
    unsigned char _pad08[8];
    unsigned long m_size;                          // +0x10
    unsigned char _pad14[5];
    bool          m_open;                          // +0x19
};
struct CLHFile {                                   // vtable 0x0129F094 over LHA::CFile; the counted pointer at +4 (object) / +8 (count)
    virtual bool GetSize(unsigned long* size);     // slot 0
    virtual void v01();
    virtual long GetMode();                        // slot 2
    SFileData_00add160* m_pData;                      // +0x04
    void*            m_pCount;                     // +0x08
};

bool CLHFile::GetSize(unsigned long* size)
{
    SFileData_00add160* data = m_pData;
    if (data && data->m_open) {
        *size = data->m_size;
        return true;
    }
    return false;
}
