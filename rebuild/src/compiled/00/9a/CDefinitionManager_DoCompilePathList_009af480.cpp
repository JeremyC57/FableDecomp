// CDefinitionManager::DoCompilePathList(const CWideStringVector&, long) -- second definition pass: for every directory spec in the path list, scan the matching files (names lower-cased), sort them, then for each file read it whole into a NUL-terminated buffer, strip // and /* */ comments, record its pathname as the current file name and compile the text
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AF480, length 605 bytes (`ret 8`).
// Callees: as CreateSymbolsFromPathList 0x009AEF60 (strings, file scan, _M_insert_overflow 0x00412330 same-TU stand-in, std::sort 0x009B8440,
//   CDiskFileWin32 0x0099AD80 / Read 0x0099A960 / Close 0x0099A920, ~CBaseClassNonCopyable 0x0099A300, GFRemoveComments 0x00A60410, CRT free 0x00BFEA14),
//   CompileString 0x009AF1E0.
// Layout: retail CDefinitionManager (PDB minus 0x10 from the 12-byte vectors ahead): CurrentFilename +0xb4. Modelled locally.
// Shape: as CreateSymbolsFromPathList, except that the (unused) function-name string is built inside the per-path loop after the
//   scan locals, and there is no symbol-table finalise tail.
// Leaf: ?DoCompilePathList@CDefinitionManager@@IAEXABV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@ABVCCharString@@@Z
// Naming wave 27 (family dm, DECISIONS_W26 D5): DoCompilePathList is protected with a const CCharString& def type,
//   passed on to the protected CompileString(const char*, const CCharString&) 0x009AF1E0 (both PDBs; its own row's spelling).
//   Was public with a long flag (QAE ..J).
// Naming wave 26 (family vws): the vector is STLport 4.6 std::vector<CWideString, allocator<CWideString> >; its grow-insert 0x00412330
//   is the protected ?_M_insert_overflow@?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@IAEXPAVCWideString@@ABV3@ABU__false_type@2@I_N@Z
//   (size_type unsigned int; c691 queue). Was CWideStringVector::_M_insert_overflow(..., const __false_type&, unsigned int, bool).
#include <stdlib.h>
#include <new>
void* __cdecl operator new[](unsigned int size);
void __cdecl operator delete[](void* p);
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString ToLower() const;                        // 0x0099ED10
    class CWideString ToWideString() const;            // 0x0099E480
};
class CWideString {
public:
    CCharString ToCharString() const;   // 0x0099B2C0 (Ego_r.pdb ?ToCharString@CWideString@@QBE?AVCCharString@@XZ)
private: public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
};
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();                                           // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CASuspendableProcess;
class CDiskFileWin32 : public CBaseClassNonCopyable { public:                         // vtable 0x0122D06C, 0x1c bytes
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
void __fastcall SortWideStrings(CWideString* first, CWideString* last);   // 0x009B8440 (std::sort<CWideString*>)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __false_type { __false_type() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { // STLport vector<CWideString>, 12 bytes
public:
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector()
    {
        for (_Tp* p = _M_start; p != _M_finish; ++p)
            p->~_Tp();
        if (_M_start)
            free(_M_start);
    }
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
    const _Tp* begin() const { return _M_start; }
    const _Tp* end() const { return _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
    _Tp& operator[](unsigned int n) { return *(_M_start + n); }
protected:
    void _M_insert_overflow(_Tp* pos, const _Tp& x, const __false_type&, unsigned int fillLen, bool atEnd);   // 0x00412330
public:
    void push_back(const _Tp& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
}

#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* pos, const _Tp& x, const std::__false_type&, unsigned int fillLen, bool atEnd)
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
protected:                                              // Ego_r.pdb / FableWin.pdb IAE
    void CompileString(const char* text, const CCharString& defType);   // 0x009AF1E0
    void DoCompilePathList(const std::vector<CWideString>& paths, const CCharString& defType);
};

void CDefinitionManager::DoCompilePathList(const std::vector<CWideString>& paths, const CCharString& defType)
{
    for (const CWideString* path = paths.begin(); path != paths.end(); ++path) {
        CWideString fileName;
        CCharString functionName("DefinitionManager : DoCompilePathList");
        CFileScanState scan;
        std::vector<CWideString> files;
        if (ScanFirstFile(*path, scan, fileName)) {
            do {
                files.push_back(((CWideString*)&fileName)->ToCharString().ToLower().ToWideString());
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
            CurrentFileName = ((CWideString*)&(file.GetPathname()))->ToCharString();
            CompileString(buffer, defType);
            delete[] buffer;
        }
    }
}