// CBankFile::WriteRecoveryFile(unsigned long, unsigned long, unsigned long) -- write the "BREC" recovery header and the entry list to the bank's recovery file
// Leaf: ?WriteRecoveryFile@CBankFile@@AAEXKKK@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CDF50, 246 bytes, thiscall, `ret 0xc`, /GS (cookie 0x0139C8A8, check 0x00BFE9F9).
// Identity: Ego_r.pdb / FableWin.pdb ?WriteRecoveryFile@CBankFile@@AAEXKKK@Z (private); its callees carry the PDB spellings
//   ?GetRecoveryFilePath@CBankFile@@AAE?AVCWideString@@XZ 0x009CBFE0 and ?InternalWriteEntryList@CBankFile@@QAEKAAVCDataOutputStream@@@Z
//   0x009CDA70 (the landed source spelled them on an invented CBankFileRuntime).
// Callees: CWideString() 0x0099AED0 / ~CWideString 0x0099B510; CBaseClassNonCopyable() 0x0099A2F0 / ~ 0x0099A300;
//   CDiskFileWin32::Open 0x0099A6A0 / Close 0x0099A920 (vtable 0x0122D06C); CFileDataOutputStream(CAFile&) 0x00994400 /
//   ~CFileDataOutputStream 0x00994430 / Write 0x00994480; CDataOutputStream::WriteULONG 0x00993EA0.
// Shape: CDiskFileWin32's default constructor (PDB ??0CDiskFileWin32@@QAE@XZ) and its virtual destructor are inline (CAFile's
//   inline ctor and implicit dtor leave no vptr stores); the five-byte signature is a `char[5] = "BREC"` initialised from the
//   string literal 0x0129B59C (`mov eax,[lit] ; mov cl,[lit+4]`), so /GS places it as the buffer directly under the cookie
//   (+0x30) -- the earlier landed memcpy-from-a-global form put it below the file object and shifted the frame by 8.
//   Virtual Write / dtor on the local stream are direct calls (static type known).
// Layout: CDiskFileWin32 0x1C (vptr, AccessType, Position, Length, WinFileHandle, Opened, Pathname: Ego_r.pdb, retail ctor
//   0x0099ADC0); CFileDataOutputStream 0xC (CDataOutputStream vptr + NewLine CCharString, File). engine/CDiskFileWin32.h,
//   CDataOutputStream.h, CFileDataOutputStream.h and CBankFile.h are flat pack(1) layouts with a __vftable data member
//   (no virtuals, no member functions), so the classes are modelled locally (header debt, NOTES.md).
#include "fable_string.h"
#include "engine/CBaseClassNonCopyable.h"

enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CASuspendableProcess;

class CAFile : public CBaseClassNonCopyable {
public:
    CAFile() { }
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags) = 0;
    virtual void Close() = 0;
};

class CDiskFileWin32 : public CAFile {
public:
    CDiskFileWin32() { Opened = false; }
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099A6A0
    virtual void Close();                                                                       // 0x0099A920
    FileAccessType AccessType;          // +0x04
    long           Position;            // +0x08
    long           Length;              // +0x0c
    void*          WinFileHandle;       // +0x10
    bool           Opened;              // +0x14
    CWideString    Pathname;            // +0x18
};

class CDataOutputStream {
public:
    virtual ~CDataOutputStream();
    void WriteULONG(unsigned long value);                       // 0x00993EA0
    CCharString NewLine;                                        // +0x04
};

class CFileDataOutputStream : public CDataOutputStream {
public:
    CFileDataOutputStream(CAFile& file);                        // 0x00994400
    virtual ~CFileDataOutputStream();                           // 0x00994430
    virtual void Write(const void* data, long length);          // 0x00994480
    CAFile* File;                                               // +0x08
};

class CBankFile {
public:
    virtual ~CBankFile();
    unsigned long InternalWriteEntryList(CDataOutputStream& stream);   // 0x009CDA70
private:
    CWideString GetRecoveryFilePath();                                  // 0x009CBFE0
    void WriteRecoveryFile(unsigned long a, unsigned long b, unsigned long c);
};

void CBankFile::WriteRecoveryFile(unsigned long a, unsigned long b, unsigned long c)
{
    CWideString path = GetRecoveryFilePath();
    CDiskFileWin32 file;
    file.Open(path, FILE_ACCESS_WRITE, 4);
    CFileDataOutputStream stream(file);
    char id[5] = "BREC";
    stream.Write(id, 4);
    stream.WriteULONG(a);
    stream.WriteULONG(b);
    stream.WriteULONG(c);
    InternalWriteEntryList(stream);
}