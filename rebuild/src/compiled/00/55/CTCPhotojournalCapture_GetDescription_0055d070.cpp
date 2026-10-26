// CTCPhotojournalCapture::GetDescription(const CWideString& filename) -- read a photojournal picture's caption: the trailing dword of the file is the caption length, the caption text sits just before it; an out-of-range length gives the empty string
// Leaf: ?GetDescription@CTCPhotojournalCapture@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055D070, 272 bytes (0x10D + int3 padding), static __fastcall: ecx = the hidden
//   CWideString result, edx = the file name; plain `ret`, returns the result pointer.
// Identity: Ego_r.pdb ?GetDescription@CTCPhotojournalCapture@@SI?AVCWideString@@ABV2@@Z (static) -- the reader paired
//   with AddDescription 0x0055CFB0 in the photojournal TU. No direct caller in the image. The catalog label
//   "NHeroInformationScreens_LoadTextFromFile" is descriptive.
// Callees: CBaseClassNonCopyable ctor 0x0099A2F0, CWideString() 0x0099AED0, CDiskFileWin32::Open(const CWideString&,
//   FileAccessType, unsigned long) 0x0099A6A0, SetPosition 0x0099ABF0, Read(void*, long, CASuspendableProcess*)
//   0x0099A960, operator new[] 0x00BFEB22, CWideString(const wchar_t*) 0x0099B6B0, operator delete[] 0x00BFEB1C,
//   Close 0x0099A920, CWideString copy 0x0099B720 (from the caption or the empty string 0x013BCA24), ~CWideString
//   0x0099B510, CBaseClass::~CBaseClass 0x0099A300.
// Layout: CDiskFileWin32 (vtable 0x0122D06C, 0x1C bytes) as the Ego_r.pdb / landed 0x006F9F90 model: AccessType +4,
//   Position +8, Length +0xC, WinFileHandle +0x10, Opened +0x14, Pathname +0x18.
// Shape: a local CDiskFileWin32 with an inline default constructor (base, vtable, Pathname, `Opened = false` in the
//   body, scheduled after Open's pushes) and the inline `~CDiskFileWin32() { Close(); }` (the vtable reset, Close,
//   ~Pathname, then only ~CBaseClass: CBaseClassNonCopyable declares no destructor); the method calls on the local
//   are direct. The caption buffer is `new wchar_t[length]` read with `length` bytes; the named caption is returned
//   by copy (no NRVO), the bad-length arm returns a copy of the empty string after its own Close().
#include <stddef.h>
void* __cdecl operator new[](unsigned int size);            // 0x00BFEB22
void __cdecl operator delete[](void* block);                // 0x00BFEB1C

class CWideString {
public:
    void* PStringData;
    CWideString();                                          // 0x0099AED0
    CWideString(const wchar_t* text);                       // 0x0099B6B0
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
};
extern const CWideString g_EmptyWideString;                 // 0x013BCA24

class CBaseClass {
public:
    virtual ~CBaseClass();                                  // 0x0099A300
};
class CBaseClassNonCopyable : public CBaseClass {
public:
    CBaseClassNonCopyable();                                // 0x0099A2F0
};

enum FileAccessType { FILE_ACCESS_TYPE_1 = 1, FILE_ACCESS_TYPE_2 = 2 };
class CASuspendableProcess;
class CDiskFileWin32 : public CBaseClassNonCopyable {      // vtable 0x0122D06C
public:
    long           AccessType;                              // +0x04
    long           Position;                                // +0x08
    long           Length;                                  // +0x0c
    void*          WinFileHandle;                           // +0x10
    bool           Opened;                                  // +0x14
    CWideString    Pathname;                                // +0x18
    CDiskFileWin32() { Opened = false; }
    ~CDiskFileWin32() { Close(); }
    void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099A6A0
    void Close();                                           // 0x0099A920
    void Read(void* data, long size, CASuspendableProcess* process = 0);   // 0x0099A960
    long GetLength() const { return Length; }
public:
    virtual void SetPosition(long);   // 0x0099ABF0
};

class CTCPhotojournalCapture {
public:
    static CWideString __fastcall GetDescription(const CWideString& filename);
};

CWideString __fastcall CTCPhotojournalCapture::GetDescription(const CWideString& filename)
{
    CDiskFileWin32 file;
    file.Open(filename, FILE_ACCESS_TYPE_2, 2);
    long fileLength = file.GetLength();
    file.SetPosition(fileLength - 4);
    long descriptionLength;
    file.Read(&descriptionLength, 4);
    if (descriptionLength >= 0 && descriptionLength <= 0x200) {
        file.SetPosition(fileLength - descriptionLength - 4);
        wchar_t* buffer = new wchar_t[descriptionLength];
        file.Read(buffer, descriptionLength);
        CWideString description(buffer);
        delete[] buffer;
        file.Close();
        return description;
    }
    file.Close();
    return g_EmptyWideString;
}