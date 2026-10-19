// CBankFile::WriteRecoveryFile(unsigned long, unsigned long, unsigned long) -- write a recovery header and entry list to the bank's recovery path
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CDF50, 246 matched bytes; __fastcall, ret 0x0c.
// Callees: CBankFile::GetRecoveryFilePath 0x009CBFE0; CDiskFileWin32::Open 0x0099A6A0;
// CFileDataOutputStream ctor/dtor 0x00994400/0x00994430; Write 0x00994480; WriteULong 0x00993EA0;
// CBankFile::InternalWriteEntryList 0x009CDA70; security cookie check 0x00BFE9F9.
// Uses the actual string and donor layout headers. The local file runtime view has the audited
// CBaseClassNonCopyable base and all sixteen named CDiskFileWin32 virtual slots. The stream method
// adapter uses the actual flat donor storage and does not synthesize a vtable.
// The recovery signature is the four-byte word plus terminator at 0x0129B59C.
// Flags: /GS
#include "engine/CBankFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CDataOutputStream.h"
#include "engine/CFileDataOutputStream.h"
#include "fable_string.h"
#include <string.h>

struct CASuspendableProcess;
struct CAFile;
struct CFileInformation;
enum FileAccessType { FILE_ACCESS_READ = 1 };

struct CBaseClassNonCopyable {
    CBaseClassNonCopyable();                 // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();        // 0x0099A300; restores base vtable
};

struct CDiskFileWin32Runtime : CBaseClassNonCopyable {
    FileAccessType AccessType;               // donor +0x04
    long Position;                           // donor +0x08
    long Length;                             // donor +0x0c
    void* WinFileHandle;                     // donor +0x10
    bool Opened;                             // donor +0x14
    unsigned char _pad_15[3];                // donor +0x15
    CWideString Pathname;                    // donor +0x18

    CDiskFileWin32Runtime() : CBaseClassNonCopyable(), Pathname() { Opened = false; }
    virtual ~CDiskFileWin32Runtime() { Close(); }
    virtual void Open(const CWideString&, FileAccessType, unsigned long); // slot 1, 0x0099A6A0
    virtual void Close();                   // slot 2, 0x0099A920
    virtual void Read(void*, long, CASuspendableProcess*); // slot 3, 0x0099A960
    virtual void Write(const void*, long, CASuspendableProcess*); // slot 4, 0x0099AA90
    virtual void SetPosition(long);         // slot 5, 0x0099ABF0
    virtual void MovePosition(long);        // slot 6, 0x0099AC90
    virtual long GetPosition() const;        // slot 7, 0x0099ABD0
    virtual void SetLength(long);           // slot 8, 0x0099A560
    virtual long GetLength() const;          // slot 9, 0x00401670
    virtual bool IsOpen() const;             // slot 10, 0x00401680
    virtual CWideString GetPathname() const; // slot 11, 0x00401690
    virtual bool IsIOFinished() const;       // slot 12, 0x00401650
    virtual long GetAlignment() const;       // slot 13, 0x00401660
    virtual CAFile* Clone() const;           // slot 14, 0x0099AE00
    virtual CFileInformation GetFileInformation(); // slot 15, 0x0099A5A0
};

struct CFileDataOutputStreamRuntime : CFileDataOutputStream {
    explicit CFileDataOutputStreamRuntime(CAFile*); // 0x00994400
    ~CFileDataOutputStreamRuntime();                 // 0x00994430
    void Write(const void*, long);                   // 0x00994480
    void WriteULong(unsigned long);                  // 0x00993EA0
};

struct RecoveryFileSignature {
    fable_u32 Word;
    fable_u8 Terminator;
};
extern const RecoveryFileSignature gBankRecoverySignature; // 0x0129B59C, bytes "BREC\0"

struct CBankFileRuntime : CBankFile {
    CWideString GetRecoveryFilePath();              // 0x009CBFE0
    unsigned long InternalWriteEntryList(CDataOutputStream&); // 0x009CDA70
    void WriteRecoveryFile(unsigned long, unsigned long, unsigned long);
};

void CBankFileRuntime::WriteRecoveryFile(unsigned long version, unsigned long state,
                                         unsigned long checksum)
{
    CWideString path = GetRecoveryFilePath();
    CDiskFileWin32Runtime file;
    file.Open(path, (FileAccessType)4, 4);
    CFileDataOutputStreamRuntime stream((CAFile*)&file);
    char signature[5];
    memcpy(signature, &gBankRecoverySignature.Word, 4);
    signature[4] = (char)gBankRecoverySignature.Terminator;
    stream.Write(signature, 4);
    stream.WriteULong(version);
    stream.WriteULong(state);
    stream.WriteULong(checksum);
    InternalWriteEntryList((CDataOutputStream&)stream);
}