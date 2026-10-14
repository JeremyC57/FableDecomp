// CLHFile::GetMode() -- map the open file data's mode word (+4 -> +4): 0 gives 0x200, 1 gives 0x800, anything else (or no / closed data) gives 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADD190, length 48 bytes.
// Vtable 0x0129F094 slot 2 (labelled vfunc_2). No callees. Shape: three immediate loads with the 1 shared by the closed path, `ret`.
struct SFileData_00add190 {                           // the counted file data object
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
    SFileData_00add190* m_pData;                      // +0x04
    void*            m_pCount;                     // +0x08
};

long CLHFile::GetMode()
{
    SFileData_00add190* data = m_pData;
    if (data && data->m_open) {
        if (data->m_mode == 0)
            return 0x200;
        if (data->m_mode == 1)
            return 0x800;
    }
    return 1;
}