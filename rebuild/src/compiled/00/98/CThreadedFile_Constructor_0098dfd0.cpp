// CThreadedFile::CThreadedFile() -- default-construct a threaded file: no handle, empty name, not open
// Leaf: ??0CThreadedFile
// Retail: Fable.exe @ 0x0098DFD0, 41 bytes. CBaseClassNonCopyable base ctor (0x0099A2F0), CThreadedFile vtable 0x0129A158 (RTTI),
// CWideString default ctor (0x0099AED0) for Filename at +0x0C, then the two flags (+0x18, +0x19, one `xor al,al`)
// and FileHandle = INVALID_HANDLE_VALUE (+0x08) last -- the flags are mem-initialisers, the handle is assigned in
// the body. DeviceID (+4), Length (+0x10) and PhysicalSortKey (+0x14) are left unset. Layout from Ego_r.pdb
// (sizeof 0x1C); no engine header exists. The catalog label NPlayerGui::CMultiStatBar::CMultiStatBar is propagated.
class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable();                                   // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};
class CWideString {
public:
    CWideString();                             // 0x0099AED0
    ~CWideString();
private:
    unsigned short* m_pData;
};
enum EThreadedFileDeviceID { THREADED_FILE_DEVICE_DEFAULT };
class CThreadedFile : public CBaseClassNonCopyable {
public:
    CThreadedFile();
    virtual ~CThreadedFile();
    EThreadedFileDeviceID DeviceID;            // +0x04
    void*                 FileHandle;          // +0x08
    CWideString           Filename;            // +0x0C
    unsigned long         Length;              // +0x10
    unsigned long         PhysicalSortKey;     // +0x14
    bool                  OpenedForWrite;      // +0x18
    bool                  OpenFlag;            // +0x19
};
CThreadedFile::CThreadedFile()
    : OpenedForWrite(false), OpenFlag(false)
{
    FileHandle = (void*)-1;                    // INVALID_HANDLE_VALUE
}
