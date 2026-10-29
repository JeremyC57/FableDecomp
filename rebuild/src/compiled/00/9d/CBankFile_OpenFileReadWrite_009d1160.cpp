// CBankFile::OpenFileReadWrite(const CWideString& filename, unsigned long flags, unsigned long alignment) -- reset the open state (OpenFlags = flags | 0x1f8), remember the name and alignment, open a new CDiskFileWin32 on it -- read-only when it already exists and flag bit 0 is clear, otherwise create it (written, solid) -- read the header when the file has data, then run the post-open step
// Leaf: ?OpenFileReadWrite@CBankFile@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D1160, length 252 bytes (`ret 0Ch`), virtual.
// Callees: CWideString::operator= @ 0x0099B7D0, operator new @ 0x00BFEA1A, CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CWideString::
//          CWideString() @ 0x0099AED0, CCountedPointer<CDiskFileWin32>::Reset @ 0x009D1EE0, PathExists @ 0x00999230
//          (__fastcall), the file's virtual slots 1 (+0x04, Open(name, access, 4)) and 9 (+0x24, the length),
//          CBankFile::ReadFileInformation(bool) @ 0x009D0880, this's virtual slot 11 (+0x2c, the post-open step).
// Layout: the landed sibling OpenFileReadOnly (0x009D1090) CBankFile model plus Alignment +0x98 (PDB +0xa8).
// Shape: the resets are written ReadOnly, OpenFlags, Alignment (the order that schedules retail's 0x98 / 0x8d / 0x78 stores; three
//          other orders move a store or the flags load); the flag test re-reads the argument
//          (`test byte ptr [esp+18h],1`); the two open arms differ only in the access mode and the trailing flag stores.
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                       // 0x0099A2F0
};
class CWideString { public:
    void* PStringData;
    CWideString();                                 // 0x0099AED0
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
class CAFile {
public:
    static bool __fastcall PathExists(const CWideString&);   // 0x00999230
};   // 0x00999230
class CDiskFileWin32 : public CBaseClassNonCopyable { public:                    // vtable 0x0122D06C
    long        AccessType;                        // +0x04
    long        Position;                          // +0x08
    long        Length;                            // +0x0c
    void*       WinFileHandle;                     // +0x10
    bool        Opened;                            // +0x14
    CWideString Pathname;                          // +0x18
    CDiskFileWin32() { Opened = false; }
    virtual void Open(const CWideString& pathname, long access, unsigned long flags);   // slot 1 (+0x04)
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual long GetLength();                      // slot 9 (+0x24)
};
template <class T> class CCountedPointer { public:
    T* Data;
    void* Info;
    void Reset(T* p);                              // 0x009D1EE0
    T* operator->() const { return Data; }
};

class CBankFile {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void PostOpen();                       // slot 11 (+0x2c)
    virtual bool OpenFileReadOnly(const CWideString& filename, unsigned long flags);
    virtual void OpenFileReadWrite(const CWideString& filename, unsigned long flags, unsigned long alignment);
    unsigned char _pad04[0x74];                    // +0x04
    unsigned long OpenFlags;                       // +0x78
    CCountedPointer<CDiskFileWin32> BankFile;      // +0x7c
    CWideString Filename;                          // +0x84
    void* BankHandle;                              // +0x88 CCharString
    bool RetailMode;                               // +0x8c
    bool ReadOnly;                                 // +0x8d
    bool WrittenToFile;                            // +0x8e
    bool Solid;                                    // +0x8f
    unsigned long ChangeCount;                     // +0x90
    bool FileValid;                                // +0x94
    bool UpdatingFlag;                             // +0x95
    unsigned long Alignment;                       // +0x98
    bool ReadFileInformation(bool validate);       // 0x009D0880
    void Close();                                  // 0x009CD850
};

void CBankFile::OpenFileReadWrite(const CWideString& filename, unsigned long flags, unsigned long alignment)
{
    ReadOnly = false;
    OpenFlags = flags | 0x1f8;
    Alignment = alignment;
    ChangeCount = 0;
    FileValid = false;
    WrittenToFile = false;
    UpdatingFlag = false;
    RetailMode = false;
    Filename = filename;
    BankFile.Reset(new CDiskFileWin32);
    if (!(flags & 1) && CAFile::PathExists(Filename)) {
        BankFile->Open(Filename, 1, 4);
        Solid = false;
    } else {
        BankFile->Open(Filename, 4, 4);
        WrittenToFile = true;
        Solid = true;
    }
    if (BankFile->GetLength() > 0)
        ReadFileInformation(true);
    PostOpen();
}