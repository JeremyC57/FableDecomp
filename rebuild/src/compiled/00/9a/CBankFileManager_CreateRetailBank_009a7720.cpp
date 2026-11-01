// CBankFileManager::CreateRetailBank(const CWideString& path) -- create an empty retail bank file: open it for writing and write the magic, the manager's file version and two zero words
// Leaf: ?CreateRetailBank@CBankFileManager@@AAEXABVCWideString@@@Z
// Naming wave 27 (family fdosw, DECISIONS_W26 D2): CFileDataOutputStream is the polymorphic retail class (engine/
//   CFileDataOutputStream.h FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM: vtable 0x0129A6F8, Write slot 4 UAE, ~ UAE). Was a non-virtual Write / ~ on a stand-in; CreateRetailBank private (AAE) as both PDBs spell it.
// Naming wave 26 (family fdos): ??0CFileDataOutputStream@@QAE@AAVCAFile@@@Z 0x00994400 (Ego_r.pdb + FableWin.pdb; c693 queue) takes the
//   file by reference; CDiskFileWin32 derives from CAFile (RTTI 0x0122D06C). Was CFileDataOutputStream(CDiskFileWin32*).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A7720, length 171 bytes (+5 int3), ret 4.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CWideString::CWideString() @ 0x0099AED0, CDiskFileWin32::Open(const CWideString&,
// FileAccessType, ulong) @ 0x0099A6A0 (access 4, flags 2), CFileDataOutputStream::CFileDataOutputStream(CAFile*) @ 0x00994400,
// CFileDataOutputStream::Write(const void*, long) @ 0x00994480 (the 4-byte magic @ 0x0129B004), CDataOutputStream::WriteULong
// @ 0x00993EA0, CFileDataOutputStream::~CFileDataOutputStream @ 0x00994430 (catalog "~CFileDataInputStream"),
// CDiskFileWin32::Close @ 0x0099A920, CWideString::~CWideString @ 0x0099B510, CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300.
// Shape: the local CDiskFileWin32's inline ctor/dtor as in CBankFile::GetLastModifiedTime (0x009CBC50; Opened cleared in the
// body); the stream is a 0xc-byte local over the file; Open/Write are direct calls on the locals.
// Layout: retail CBankFileManager = Ego_r.pdb with 12-byte maps and a 4-byte list: Banks +0x04, RetailBanks +0x10,
// UsingRetailBanks +0x14, BankAliasMap +0x18, BasePath +0x24, CreatedRetailBanks +0x28, FileVersion +0x34.
class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable();                                       // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                                      // 0x0099A300
};
class CWideString { public:
    void* PStringData;
    CWideString();                                 // 0x0099AED0
    ~CWideString();                                // 0x0099B510
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CAFile : public CBaseClassNonCopyable { };   // RTTI 0x0122D06C chain: CDiskFileWin32 <- CAFile <- CBaseClassNonCopyable
class CDiskFileWin32 : public CAFile { public:                    // vtable 0x0122D06C
    long        AccessType;                        // +0x04
    long        Position;                          // +0x08
    long        Length;                            // +0x0c
    void*       WinFileHandle;                     // +0x10
    bool        Opened;                            // +0x14
    CWideString Pathname;                          // +0x18
    CDiskFileWin32() { Opened = false; }
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099A6A0
    virtual void Close();                          // 0x0099A920
};
class CDataStream : public CBaseClassNonCopyable { public:      // RTTI .?AVCDataStream@@ (vtable 0x0129A69C)
    virtual ~CDataStream();                                     // ??1CDataStream@@UAE@XZ
};
class CDataOutputStream : public CDataStream { public:          // vtable 0x0129A6E0 (slots 1-4 _purecall)
    void* NewLine;                                              // +0x04 CCharString NewLine (opaque here)
    virtual void SetPosition(unsigned long position) = 0;      // slot 1
    virtual unsigned long GetPosition() const = 0;              // slot 2
    virtual void Close() = 0;                                   // slot 3
    virtual void Write(const void* data, long length) = 0;      // slot 4
    void WriteULONG(unsigned long);                             // 0x00993EA0
};
#define FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM
#include "engine/CFileDataOutputStream.h"                      // the retail layout (vtable 0x0129A6F8)
extern const unsigned char g_retailBankMagic[4];   // 0x0129B004

class CBankFileManager {
public:
    unsigned char _pad[0x34];
    unsigned long FileVersion;                     // +0x34
private:
    void CreateRetailBank(const CWideString& path);   // private (Ego_r.pdb / FableWin.pdb AAE)
};

void CBankFileManager::CreateRetailBank(const CWideString& path)
{
    CDiskFileWin32 file;
    file.Open(path, FILE_ACCESS_WRITE, 2);
    CFileDataOutputStream stream(file);
    stream.Write(g_retailBankMagic, 4);
    stream.WriteULONG(FileVersion);
    stream.WriteULONG(0);
    stream.WriteULONG(0);
}