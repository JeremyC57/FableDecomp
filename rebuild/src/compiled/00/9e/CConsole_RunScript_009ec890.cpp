// CConsole::RunScript(const CWideString& filename) -- run a console script file: when the file exists read it whole into a NUL-terminated char buffer, strip // (to \n, \r or the end) and /* */ comments, and hand the text to RunTextCommand
// Leaf: ?RunScript@CConsole@@QAEXABVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EC890, 402 bytes, thiscall, `ret 4`.
// Identity: Ego_r.pdb `?RunScript@CConsole@@QAEXABVCWideString@@@Z`; donor order RunTextCommand (landed 0x009EC710),
//   RunScript, ProcessBindKeyInput matches retail, and the landed console command CConsole::ConsoleRunScript 0x009ECB70
//   calls this address (0x009ECC53). Every caller binds the invented `?ExecuteConfigFile@CConsole@@QAEXABVCWideString@@@Z`
//   (in neither PDB; NAME_FIXES.tsv).
// Callees: CBaseClassNonCopyable() 0x0099A2F0 / ~ 0x0099A300, CWideString() 0x0099AED0 / ~ 0x0099B510 (the inlined
//   CDiskFileWin32 default ctor and dtor, vtable 0x0122D06C), CAFile::PathExists 0x00999230 (static __fastcall),
//   CDiskFileWin32::Open 0x0099A6A0 / Read 0x0099A960 / Close 0x0099A920 (direct: the file is a local of known type),
//   vector<char>(n) 0x00411910 (inside the retail CArray<char>), GFRemoveComments 0x00A60410 (x4), CCharString(const char*, long) 0x0099EBF0 / () 0x0099E4B0 /
//   ~ 0x0099EAE0, CStringParser(const char*, callback, const CCharString&, long) 0x00414E30 with its inlined dtor
//   (~list<CStringParserCommentBoundaries> 0x004046B0, ~CBaseClass 0x0099A2E0), CConsole::RunTextCommand 0x009EC710, free.
// Shape: an early `return` when the path is missing (the hoisted `lea ecx, file` before the branch: the if-block form
//   recomputes it after the Open pushes); the default ctor assigns Opened in its body (after the Pathname ctor); the parser
//   and a CParsedItem are built and left unused (as in RunTextCommand's loop prologue); the text is the buffer's
//   GetAsCArray() (its own address when empty) converted implicitly to the const CCharString& parameter (a frame
//   temporary passed by `lea`; an explicit CCharString(...) passes the constructor's eax).
// Literals: "//" 0x0129B204, "\n" 0x0129B208, "\r" 0x0129B200, "\0" 0x0129C5F0, "/*" 0x0129B1F8, "*/" 0x0129B1FC, "" 0x0122D70E.
// Layout: Ego_r.pdb CDiskFileWin32 (0x1c) and CStringParser (retail 0x28: STLport list is 4 bytes); CParsedItem as the
//   landed RunTextCommand 0x009EC710.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
extern "C" void* __cdecl memset(void*, int, unsigned int);
#pragma intrinsic(memset)
class CCharString { public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
};
class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable();                            // 0x0099A2F0 (Ego_r.pdb ??0CBaseClassNonCopyable@@IAE@XZ)
    virtual ~CBaseClassNonCopyable();                   // 0x0099A300 (Ego_r.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CASuspendableProcess;
class CDiskFileWin32 : public CBaseClassNonCopyable { public:     // vtable 0x0122D06C, 0x1c bytes
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099A6A0
    virtual void Close();                               // 0x0099A920
    virtual void Read(void* data, long size, CASuspendableProcess* process);   // 0x0099A960
    FileAccessType AccessType;                          // +0x04
    long        Position;                               // +0x08
    long        Length;                                 // +0x0c
    void*       WinFileHandle;                          // +0x10
    bool        Opened;                                 // +0x14
    CWideString Pathname;                               // +0x18
    CDiskFileWin32() { Opened = false; }
    virtual ~CDiskFileWin32() { Close(); }
};
class CAFile { public:
    static bool __fastcall PathExists(const CWideString& pathname);   // 0x00999230
};
void __fastcall GFRemoveComments(char* text, const char* start, const char* end);   // 0x00A60410

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CStringParserCommentBoundaries;
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc = allocator<_Tp> > class vector;
template <class _Tp, class _Alloc = allocator<_Tp> > class list { public:   // STLport list: the sentinel node pointer
    void* _M_node;
    ~list();                                            // 0x004046B0
};
}
struct CCursorPos {
    bool          InQuotes;
    unsigned long StringPos;
    unsigned long Line;
};
class CStringParser : public CBaseClass { public:       // retail 0x28 bytes
    char*                    String;                    // +0x04
    unsigned long            BaseLineOffset;            // +0x08
    CCharString              Filename;                  // +0x0c
    CCharString              TempCharString;            // +0x10
    std::list<CStringParserCommentBoundaries> Comments;  // +0x14
    void (__fastcall *ErrorCallback)(const CCharString&);   // +0x18
    CCursorPos               CursorPos;                 // +0x1c
    CStringParser(const char* text, void (__fastcall *errorCallback)(const CCharString&), const CCharString& filename,
                  long baseLineOffset);                 // 0x00414E30
};
class CParsedItem { public:                             // 12 bytes
    long        Type;
    long        Value;
    CCharString Text;
};
namespace std {
template <class _Tp> class allocator { public: };
template <class _Tp, class _Alloc> class vector { public:
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
    explicit vector(unsigned int __n);                  // 0x00411910
    ~vector() { if (_M_start) free(_M_start); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp& operator[](unsigned int __n) { return *(_M_start + __n); }
    const _Tp& operator[](unsigned int __n) const { return *(_M_start + __n); }
};
}
template <class T> class CArray : public std::vector<T> {  // retail CArray<T>: the STLport vector plus the engine helpers
public:
    explicit CArray(unsigned int size) : std::vector<T>(size) { }
    T* GetAsCArray() { return this->size() != 0 ? &(*this)[0] : GetZeroSizeArray(); }   // FableWin.pdb ?GetAsCArray@?$CArray@D@@QAEPADXZ
private:
    T* GetZeroSizeArray() { return (T*)this; }          // FableWin.pdb ?GetZeroSizeArray@?$CArray@D@@AAEPADXZ
};

class CConsole { public:
    void RunTextCommand(const CCharString& text);       // 0x009EC710
    void RunScript(const CWideString& filename);
};

void CConsole::RunScript(const CWideString& filename)
{
    CDiskFileWin32 file;
    if (!CAFile::PathExists(filename))
        return;
    file.Open(filename, FILE_ACCESS_READ, 2);
    CArray<char> buffer(file.Length + 1);
    file.Read(&buffer[0], file.Length, 0);
    buffer[file.Length] = 0;
    GFRemoveComments(&buffer[0], "//", "\n");
    GFRemoveComments(&buffer[0], "//", "\r");
    GFRemoveComments(&buffer[0], "//", "\0");             // 0x0129C5F0: a 4-aligned literal of zero bytes (not the shared "" 0x0122D70E)
    GFRemoveComments(&buffer[0], "/*", "*/");
    CStringParser parser(&buffer[0], 0, CCharString(""), 0);
    CParsedItem item;
    RunTextCommand(buffer.GetAsCArray());
}