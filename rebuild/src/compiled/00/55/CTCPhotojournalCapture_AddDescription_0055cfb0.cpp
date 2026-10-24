// CTCPhotojournalCapture::AddDescription(const CWideString& filename, const CWideString& description) -- append a caption to a saved photojournal screenshot: open the file read/write, seek to its end, write the caption's UTF-16 characters, then its character count as a trailing long, close
// Leaf: ?AddDescription@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055CFB0, 178 bytes (192 with padding), plain `ret` (static /Gr __fastcall:
//   filename in ecx, description in edx). No direct callers found (xref.py: 0); the reader is its twin
//   GetDescription 0x0055D070 (seek to end - 4, read the count, accept 0..0x200).
// Callees: CBaseClassNonCopyable() 0x0099A2F0, CWideString() 0x0099AED0, CDiskFileWin32::Open 0x0099A6A0 /
//   SetPosition 0x0099ABF0 / Write 0x0099AA90 / Close 0x0099A920 (direct calls: the local's dynamic type is known),
//   CWideString::operator const wchar_t* 0x0099AF10, CWideString::GetLength 0x0099B220, ~CWideString 0x0099B510,
//   ~CBaseClassNonCopyable 0x0099A300.
// Label correction: catalogued as unscoped "AddDescription"; Ego_r.pdb
//   `?AddDescription@CTCPhotojournalCapture@@SIXABVCWideString@@0@Z` (static, __fastcall).
// Layout: the local CDiskFileWin32 view is the landed CBankFile::WriteRecoveryFile 0x009CDF50 runtime model (named
//   virtual slots over the donor fields; engine/CDiskFileWin32.h is flat donor storage without the base/vtable), with
//   the slot-9 GetLength defined inline (`mov eax,[file.Length]`). Access value 2 has no recovered enumerator name
//   (1 = read, 4 = write in the landed rows); FILE_ACCESS_READ_WRITE is descriptive.
// Shape: the default constructor stores Opened = false after the Pathname constructor (scheduled among the Open
//   pushes); the text pointer is taken into a local before the length call (edi), and the count is a second
//   GetLength into a stack long; the explicit Close() precedes the inline destructor's own Close().
#include "fable_string.h"

struct CASuspendableProcess;
struct CAFile;
struct CFileInformation;
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_READ_WRITE = 2, FILE_ACCESS_WRITE = 4 };

struct CBaseClassNonCopyable {
    CBaseClassNonCopyable();                 // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();        // 0x0099A300
};

struct CDiskFileWin32 : CBaseClassNonCopyable {   // vtable 0x0122D06C, 0x1c bytes
    FileAccessType AccessType;               // +0x04
    long Position;                           // +0x08
    long Length;                             // +0x0c
    void* WinFileHandle;                     // +0x10
    bool Opened;                             // +0x14
    CWideString Pathname;                    // +0x18

    CDiskFileWin32() : CBaseClassNonCopyable(), Pathname() { Opened = false; }
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Open(const CWideString&, FileAccessType, unsigned long); // slot 1, 0x0099A6A0
    virtual void Close();                   // slot 2, 0x0099A920
    virtual void Read(void*, long, CASuspendableProcess*); // slot 3, 0x0099A960
    virtual void Write(const void*, long, CASuspendableProcess*); // slot 4, 0x0099AA90
    virtual void SetPosition(long);         // slot 5, 0x0099ABF0
    virtual void MovePosition(long);        // slot 6, 0x0099AC90
    virtual long GetPosition() const;        // slot 7, 0x0099ABD0
    virtual void SetLength(long);           // slot 8, 0x0099A560
    virtual long GetLength() const { return Length; }   // slot 9, 0x00401670
    virtual bool IsOpen() const;             // slot 10, 0x00401680
    virtual CWideString GetPathname() const; // slot 11, 0x00401690
    virtual bool IsIOFinished() const;       // slot 12, 0x00401650
    virtual long GetAlignment() const;       // slot 13, 0x00401660
    virtual CAFile* Clone() const;           // slot 14, 0x0099AE00
    virtual CFileInformation GetFileInformation(); // slot 15, 0x0099A5A0
};

class CTCPhotojournalCapture {
public:
    static void __fastcall AddDescription(const CWideString& filename, const CWideString& description);
};

void __fastcall CTCPhotojournalCapture::AddDescription(const CWideString& filename, const CWideString& description)
{
    CDiskFileWin32 file;
    file.Open(filename, FILE_ACCESS_READ_WRITE, 2);
    file.SetPosition(file.GetLength());
    const wchar_t* text = description;
    file.Write(text, description.GetLength() * sizeof(wchar_t), 0);
    long length = description.GetLength();
    file.Write(&length, sizeof(length), 0);
    file.Close();
}