// CCachedFile::OpenFromFile(CAFile* file, long mode) -- adopt an open disk file: its pathname, its length as the position, flags, and a shared reference to the file
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5B210, length 77 bytes (+ int3 padding).
// Callees: CAFile virtual slot 11 (+0x2c, GetPathname: a CWideString returned through the hidden
// pointer), CWideString::operator= @ 0x0099B7D0, CWideString::~CWideString @ 0x0099B510, CAFile
// slot 7 (+0x1c, GetLength), slot 14 (+0x38, GetSharedFile), CCountedPointer<CAFile>::Reset
// @ 0x0050F550 (landed: CCountedPointer_0050be40::Reset).
// Layout (Ego_r.pdb CCachedFile with the retail 12-byte CArray, so the PDB offsets after it sit
// 4 bytes high): Position +0x04, BufferStartPosition +0x08, DataBuffer +0x0c, Pathname +0x18,
// PDiskFile +0x1c, Opened +0x24, BufferValid +0x25. The `mode` argument is unused.
// Shape: the pathname temporary is homed in `file`'s dead argument slot (the pointer was loaded
// into edi first); the file's vtable is re-read before every virtual call.
class CWideString { public:
    void* m_pStringData;
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    ~CWideString();                             // 0x0099B510
};

class CAFile { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual long GetLength();                   // slot 7 (+0x1c)
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual CWideString GetPathname();          // slot 11 (+0x2c)
    virtual void v12();
    virtual void v13();
    virtual CAFile* GetSharedFile();            // slot 14 (+0x38)
};

struct CCountedPointer_CAFile {                 // {object, count block}
    CAFile* m_pObject;
    void*   m_pCount;
    void Reset(CAFile* file);                   // 0x0050F550
};

class CCachedFile { public:
    void*                 _vptr;                // +0x00
    long                  Position;             // +0x04
    long                  BufferStartPosition;  // +0x08
    unsigned char         DataBuffer[0x0c];     // +0x0c CArray<unsigned char>
    CWideString           Pathname;             // +0x18
    CCountedPointer_CAFile PDiskFile;           // +0x1c
    bool                  Opened;               // +0x24
    bool                  BufferValid;          // +0x25
    void OpenFromFile(CAFile* file, long mode);
};

void CCachedFile::OpenFromFile(CAFile* file, long mode)
{
    Pathname = file->GetPathname();
    Position = file->GetLength();
    Opened = true;
    BufferValid = false;
    PDiskFile.Reset(file->GetSharedFile());
}