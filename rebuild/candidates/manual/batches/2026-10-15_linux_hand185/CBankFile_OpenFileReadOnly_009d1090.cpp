// CBankFile::OpenFileReadOnly(const CWideString& filename, unsigned long flags) -- reset the open state, then (if the file exists) open it read-only on a new CDiskFileWin32, remember the name and read the bank header; failure closes the bank
// Leaf: ?OpenFileReadOnly@CBankFile@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D1090, length 204 bytes (+4 int3), virtual, ret 8, returns bool.
// Callees: PathExists(const CWideString&) @ 0x00999230 (__fastcall), operator new @ 0x00BFEA1A, CBase::CBase @ 0x0099A2F0,
// CWideString::CWideString() @ 0x0099AED0, CCountedPointer<CDiskFileWin32>::Reset(CDiskFileWin32*) @ 0x009D1EE0 (catalog
// "CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset", a byte-twin), the file's virtual slot 1 (+0x04, Open(name,
// read 1, flags 4)), CWideString::operator= @ 0x0099B7D0, CBankFile::ReadFileInformation(bool) @ 0x009D0880, CBankFile::Close
// @ 0x009CD850 (catalog "CDefStringTable::Destructor", propagated), this's virtual slot 11 (+0x2c, the post-open step).
// Shape: the resets use the zero register ebx, which then doubles as the null result of the `new` (null test, inline
// CDiskFileWin32 ctor: vtable 0x0122D06C, Pathname, Opened = false in the body). The bool argument is `!(flags & 2)`
// computed as `~(flags >> 1) & 1` in al.
// Layout: the landed retail CBankFile model (0x009CC240): OpenFlags +0x78, BankFile (CCountedPointer<CDiskFileWin32>) +0x7c,
// Filename +0x84, RetailMode +0x8c, ReadOnly +0x8d, WrittenToFile +0x8e, Solid +0x8f, ChangeCount +0x90, FileValid +0x94,
// UpdatingFlag +0x95.
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};
struct CWideString {
    void* PStringData;
    CWideString();                                 // 0x0099AED0
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
bool __fastcall PathExists(const CWideString& path);   // 0x00999230
struct CDiskFileWin32 : CBase {                    // vtable 0x0122D06C
    long        AccessType;                        // +0x04
    long        Position;                          // +0x08
    long        Length;                            // +0x0c
    void*       WinFileHandle;                     // +0x10
    bool        Opened;                            // +0x14
    CWideString Pathname;                          // +0x18
    CDiskFileWin32() { Opened = false; }
    virtual void Open(const CWideString& pathname, long access, unsigned long flags);   // slot 1 (+0x04)
};
template <class T> struct CCountedPointer {
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
    bool ReadFileInformation(bool validate);       // 0x009D0880
    void Close();                                  // 0x009CD850
};

bool CBankFile::OpenFileReadOnly(const CWideString& filename, unsigned long flags)
{
    ReadOnly = true;
    OpenFlags = flags;
    ChangeCount = 0;
    FileValid = false;
    WrittenToFile = false;
    Solid = false;
    UpdatingFlag = false;
    RetailMode = false;
    if (!PathExists(filename))
        return false;
    BankFile.Reset(new CDiskFileWin32);
    BankFile->Open(filename, 1, 4);
    Filename = filename;
    if (!ReadFileInformation(!(flags & 2))) {
        Close();
        return false;
    }
    PostOpen();
    return true;
}
