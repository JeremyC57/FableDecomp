// CBankFile::BeginUpdateEntries() -- start an update pass: a retail-mode bank reopens its file for writing at the end, pads it to the bank alignment and marks it solid; otherwise open the side-by-side ".cks" checksum cache (once) and write the file information block
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CE8D0, 407 bytes, no arguments.
// Callees: operator new @ 0x00BFEA1A, CBaseClassNonCopyable ctor @ 0x0099A2F0, CWideString::CWideString() @ 0x0099AED0,
//          CCountedPointer<CDiskFileWin32>::Reset @ 0x009D1EE0, the file's virtual slots 1 (+0x04 Open(name, access, flags)),
//          5 (+0x14 Seek) and 9 (+0x24 GetLength), CBankFile::WriteZeros(unsigned long) @ 0x009CCB20 (manifest label
//          "MakeWritable" is wrong: MakeWritable is 0x009CBF90; landed WriteEntryData 0x009CD330 names it WriteZeros),
//          CWideString::GetLength @ 0x0099B220, SplitPath @ 0x00997620 (__fastcall), operator+(const CWideString&,
//          const CWideString&) @ 0x0099BE70 and operator+(const CWideString&, const wchar_t*) @ 0x0099BF30 (__fastcall,
//          hidden result in ecx), CChecksumCacheFile::Open(const CWideString&) @ 0x00A60D40, CWideString::~CWideString
//          @ 0x0099B510, CBankFile::WriteFileInformation(bool) @ 0x009CE370. Literal L".cks" @ 0x0129B298.
// Layout: the retail CBankFile layout of the landed rows (OpenFileReadWrite 0x009D1160, Align 0x009CBB90; engine/CBankFile.h
//         keeps the PDB offsets): BankFile +0x7C, Filename +0x84, RetailMode +0x8C, WrittenToFile +0x8E, Solid +0x8F,
//         UpdatingFlag +0x95, Alignment +0x98, ChecksumCache +0xF8 (its first member is the CWideString Filename).
//         CDiskFileWin32 as in the landed OpenFileReadWrite (0x1C bytes, Opened +0x14, Pathname +0x18).
// Shape: the retail-mode arm returns with its own epilogue; Align(length) - length is the inline CBankFile::Align; the
//        path parts are locals in SplitPath's argument order (drive, directory, name, extension) inside the `if`.
typedef unsigned short wchar16;
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable();
    CBaseClassNonCopyable();                       // 0x0099A2F0
};

class CWideString {
public:
    void* PStringData;
    CWideString();                                 // 0x0099AED0
    CWideString(const CWideString& other);
    ~CWideString();                                // 0x0099B510
    long GetLength() const;                        // 0x0099B220
};
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name, CWideString* extension);   // 0x00997620
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& a, const wchar16* b);       // 0x0099BF30

struct CDiskFileWin32 : CBaseClassNonCopyable {    // vtable 0x0122D06C
    long        AccessType;                        // +0x04
    long        Position;                          // +0x08
    long        Length;                            // +0x0c
    void*       WinFileHandle;                     // +0x10
    bool        Opened;                            // +0x14
    CWideString Pathname;                          // +0x18
    CDiskFileWin32() { Opened = false; }
    virtual void Open(const CWideString& pathname, long access, unsigned long flags);   // slot 1 (+0x04)
    virtual void v02(); virtual void v03(); virtual void v04();
    virtual void Seek(long position);              // slot 5 (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual long GetLength();                      // slot 9 (+0x24)
};

template <class T> struct CCountedPointer {
    T* Data;
    void* Info;
    void Reset(T* p);                              // 0x009D1EE0
    T* operator->() const { return Data; }
};

struct CChecksumCacheFile {
    CWideString Filename;                          // +0x00
    long _body[6];
    void Open(const CWideString& path);            // 0x00A60D40
};

struct CBankFile {
    virtual ~CBankFile();
    unsigned char _pad04[0x74];                    // +0x04 Size, the four CArrays, the four packed arrays
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
    unsigned char _pad9c[0x5c];                    // +0x9c .. +0xf8
    CChecksumCacheFile ChecksumCache;              // +0xf8
    unsigned long Align(unsigned long size) const { return (Alignment + size - 1) / Alignment * Alignment; }
    void WriteZeros(unsigned long count);          // 0x009CCB20
    void WriteFileInformation(bool flush);         // 0x009CE370
    void BeginUpdateEntries();
};

void CBankFile::BeginUpdateEntries()
{
    if (RetailMode) {
        BankFile.Reset(new CDiskFileWin32);
        BankFile->Open(Filename, 2, 4);
        WrittenToFile = true;
        long length = BankFile->GetLength();
        BankFile->Seek(length);
        WriteZeros(Align(length) - length);
        BankFile->GetLength();
        Solid = true;
        UpdatingFlag = true;
        return;
    }
    if (ChecksumCache.Filename.GetLength() <= 0) {
        CWideString drive;
        CWideString directory;
        CWideString name;
        CWideString extension;
        SplitPath(Filename, &drive, &directory, &name, &extension);
        ChecksumCache.Open(drive + directory + name + L".cks");
    }
    WriteFileInformation(false);
    UpdatingFlag = true;
}