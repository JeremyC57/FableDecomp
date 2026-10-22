// CDefinitionManager::CreateSymbolsFromPathList(const CWideStringVector&, long) -- first definition pass: for every directory spec in the path list, scan the matching files (names lower-cased), sort them, then for each file read it whole into a NUL-terminated buffer, strip // and /* */ comments, record its pathname as the current file name and parse the text for symbols; finally finalise both symbol tables
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AEF60, length 625 bytes (`ret 8`).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 / operator= 0x0099EFB0 / ToLower 0x0099ED10 / ToWideString 0x0099E480,
//   CWideString() 0x0099AED0 / copy ctor 0x0099B720 / ~CWideString 0x0099B510 / ConvertToCharString 0x0099B2C0, ScanFirstFile 0x00999C50 /
//   ScanNextFile 0x00999CB0 (__fastcall), vector<CWideString>::_M_insert_overflow 0x00412330 (same-TU stand-in body below), std::sort
//   0x009B8440 (__fastcall), CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) 0x0099AD80 / Read 0x0099A960 / Close 0x0099A920,
//   ~CBase 0x0099A300, operator new[] / delete[], GFRemoveComments 0x00A60410 (__fastcall), ParseStringForSymbols 0x009ADE90, symbol-table
//   finalise 0x009E5C30, CRT free 0x00BFEA14.
// Layout: retail CDefinitionManager (PDB minus 0x10 from the 12-byte vectors ahead): symbol tables +0x58 / +0x60, CurrentFilename +0xb4.
//   CDiskFileWin32 0x1c bytes (vtable 0x0122D06C, Length +0x0c, Pathname +0x18). Modelled locally.
// Shape: CWideStringVector is a non-template STLport vector (12 bytes) with an inline dtor (destroy loop, then free) and an inline
//   placement-new push_back; _M_insert_overflow is a same-TU noinline stand-in that reads Start/Finish before writing, so the vector stops
//   escaping and _M_start/_M_finish stay in edi/esi as in retail. The chained ConvertToCharString().ToLower().ToWideString() pre-pushes
//   every hidden-return slot; Read is called qualified to get the direct call.
// Leaf: CreateSymbolsFromPathList
#include <stdlib.h>
#include <new>
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString ToLower() const;                        // 0x0099ED10
    struct CWideString ToWideString() const;            // 0x0099E480
};
struct CWideString {
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    CCharString ConvertToCharString() const;            // 0x0099B2C0
};
struct CBase {
    virtual void v00();
    ~CBase();                                           // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CASuspendableProcess;
struct CDiskFileWin32 : CBase {                         // vtable 0x0122D06C, 0x1c bytes
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // slot 1
    virtual void Close();                               // 0x0099A920 (slot 2)
    virtual void Read(void* data, long size, CASuspendableProcess* process);   // 0x0099A960 (slot 3)
    long        AccessType;                             // +0x04
    long        Position;                               // +0x08
    long        Length;                                 // +0x0c
    void*       WinFileHandle;                          // +0x10
    bool        Opened;                                 // +0x14
    CWideString Pathname;                               // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
    CWideString GetPathname() const { return Pathname; }
};
struct CFileScanState { long _body[2]; };
bool __fastcall ScanFirstFile(const CWideString& directory, CFileScanState& state, CWideString& fileName);   // 0x00999C50
bool __fastcall ScanNextFile(const CWideString& directory, CFileScanState& state, CWideString& fileName);    // 0x00999CB0
void __fastcall GFRemoveComments(char* text, const char* start, const char* end);   // 0x00A60410
struct __false_type { __false_type() {} };
void __fastcall SortWideStrings(CWideString* first, CWideString* last);   // 0x009B8440 (std::sort<CWideString*>)
class CWideStringVector {                               // STLport vector<CWideString>, 12 bytes
public:
    CWideString* _M_start;
    CWideString* _M_finish;
    CWideString* _M_end_of_storage;
    CWideStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CWideStringVector()
    {
        for (CWideString* p = _M_start; p != _M_finish; ++p)
            p->~CWideString();
        if (_M_start)
            free(_M_start);
    }
    CWideString* begin() { return _M_start; }
    CWideString* end() { return _M_finish; }
    const CWideString* begin() const { return _M_start; }
    const CWideString* end() const { return _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
    CWideString& operator[](unsigned int n) { return *(_M_start + n); }
    void _M_insert_overflow(CWideString* pos, const CWideString& x, const __false_type&, unsigned int fillLen, bool atEnd);   // 0x00412330
    void push_back(const CWideString& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CWideString(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
#pragma auto_inline(off)
void CWideStringVector::_M_insert_overflow(CWideString* pos, const CWideString& x, const __false_type&, unsigned int fillLen, bool atEnd)
{
    unsigned int oldSize = _M_finish - _M_start;
    unsigned int len = oldSize + (oldSize > fillLen ? oldSize : fillLen);
    CWideString* newStart = (CWideString*)malloc(len * sizeof(CWideString));
    CWideString* newFinish = newStart;
    CWideString* p;
    for (p = _M_start; p != pos; ++p, ++newFinish)
        new (newFinish) CWideString(*p);
    for (unsigned int n = 0; n < fillLen; ++n, ++newFinish)
        new (newFinish) CWideString(x);
    if (!atEnd)
        for (p = pos; p != _M_finish; ++p, ++newFinish)
            new (newFinish) CWideString(*p);
    for (p = _M_start; p != _M_finish; ++p)
        p->~CWideString();
    if (_M_start)
        free(_M_start);
    _M_start = newStart;
    _M_finish = newFinish;
    _M_end_of_storage = newStart + len;
}
#pragma auto_inline(on)
struct CSymbolTable { void Finalise(); };              // 0x009E5C30

class CDefinitionManager {
public:
    unsigned char _pad000[0x58];
    CSymbolTable* Symbols58;                            // +0x58
    unsigned char _pad05c[4];
    CSymbolTable* Symbols60;                            // +0x60
    unsigned char _pad064[0xb4 - 0x64];
    CCharString   CurrentFileName;                      // +0xb4
    void ParseStringForSymbols(char* text, long flags);   // 0x009ADE90
    void CreateSymbolsFromPathList(const CWideStringVector& paths, long flags);
};

void CDefinitionManager::CreateSymbolsFromPathList(const CWideStringVector& paths, long flags)
{
    CCharString functionName("DefinitionManager : CreateSymbolsFromPathList");
    for (const CWideString* path = paths.begin(); path != paths.end(); ++path) {
        CWideString fileName;
        CFileScanState scan;
        CWideStringVector files;
        if (ScanFirstFile(*path, scan, fileName)) {
            do {
                files.push_back(fileName.ConvertToCharString().ToLower().ToWideString());
            } while (ScanNextFile(*path, scan, fileName));
        }
        long count = files.size();
        SortWideStrings(files.begin(), files.end());
        for (long i = 0; i < count; ++i) {
            CDiskFileWin32 file(files[i], FILE_ACCESS_READ, 2);
            char* buffer = new char[file.Length + 1];
            file.CDiskFileWin32::Read(buffer, file.Length, 0);
            buffer[file.Length] = 0;
            GFRemoveComments(buffer, "//", "\n");
            GFRemoveComments(buffer, "//", "\r");
            GFRemoveComments(buffer, "/*", "*/");
            CurrentFileName = file.GetPathname().ConvertToCharString();
            ParseStringForSymbols(buffer, flags);
            delete[] buffer;
        }
    }
    Symbols58->Finalise();
    Symbols60->Finalise();
}
