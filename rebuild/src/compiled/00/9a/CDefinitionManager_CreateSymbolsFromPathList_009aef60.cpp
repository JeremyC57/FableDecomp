// CDefinitionManager::CreateSymbolsFromPathList(const CWideStringVector&, long) -- first definition pass: for every directory spec in the path list, scan the matching files (names lower-cased), sort them, then for each file read it whole into a NUL-terminated buffer, strip // and /* */ comments, record its pathname as the current file name and parse the text for symbols; finally finalise both symbol tables
// Naming (agentNF31, 2026-11-12): CCharString::ToLower (0x0099ED10) respelled GetAsLowerCase: Ego_r.pdb 0x00AFA690 + FableWin.pdb
//   0x02F61580 ?GetAsLowerCase@CCharString@@QBE?AV1@XZ (fable_string.h declares it at 0x0099ED10).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AEF60, length 625 bytes (`ret 8`).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 / operator= 0x0099EFB0 / ToLower 0x0099ED10 / ToWideString 0x0099E480,
//   CWideString() 0x0099AED0 / copy ctor 0x0099B720 / ~CWideString 0x0099B510 / ConvertToCharString 0x0099B2C0, ScanFirstFile 0x00999C50 /
//   ScanNextFile 0x00999CB0 (__fastcall), vector<CWideString>::_M_insert_overflow 0x00412330 (same-TU stand-in body below), std::sort
//   0x009B8440 (__fastcall), CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) 0x0099AD80 / Read 0x0099A960 / Close 0x0099A920,
//   ~CBaseClassNonCopyable 0x0099A300, operator new[] / delete[], GFRemoveComments 0x00A60410 (__fastcall), ParseStringForSymbols 0x009ADE90, symbol-table
//   finalise 0x009E5C30, CRT free 0x00BFEA14.
// Layout: retail CDefinitionManager (PDB minus 0x10 from the 12-byte vectors ahead): symbol tables +0x58 / +0x60, CurrentFilename +0xb4.
//   CDiskFileWin32 0x1c bytes (vtable 0x0122D06C, Length +0x0c, Pathname +0x18). Modelled locally.
// Shape: CWideStringVector is a non-template STLport vector (12 bytes) with an inline dtor (destroy loop, then free) and an inline
//   placement-new push_back; _M_insert_overflow is a same-TU noinline stand-in that reads Start/Finish before writing, so the vector stops
//   escaping and _M_start/_M_finish stay in edi/esi as in retail. The chained ConvertToCharString().ToLower().ToWideString() pre-pushes
//   every hidden-return slot; Read is called qualified to get the direct call.
// Leaf: ?CreateSymbolsFromPathList@CDefinitionManager@@IAEXABV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@_N@Z
// Naming wave 27 (family dm, DECISIONS_W26 D5): CreateSymbolsFromPathList is protected with a bool flag, passed on to
//   the protected ParseStringForSymbols(const char*, bool) 0x009ADE90 (both PDBs). Was public with a long flag (QAE ..J).
//   The two symbol maps (Ego_r PSymbolMap / PDefinitionSymbolMap) end with CSymbolMap::UpdateDirty 0x009E5C30 (both PDBs,
//   const; chunk 695), which this row had spelled ?Finalise@CSymbolTable (no Finalise in either PDB).
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
    CCharString GetAsLowerCase() const;                        // 0x0099ED10
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
class CSymbolMap { public: void UpdateDirty() const; };   // 0x009E5C30 (Ego_r.pdb / FableWin.pdb ?UpdateDirty@CSymbolMap@@QBEXXZ)

class CDefinitionManager {
public:
    unsigned char _pad000[0x58];
    CSymbolMap*   PSymbolMap;                           // +0x58 CCountedPointer<CSymbolMap> PSymbolMap (its pointer word)
    unsigned char _pad05c[4];
    CSymbolMap*   PDefinitionSymbolMap;                 // +0x60 CCountedPointer<CSymbolMap> PDefinitionSymbolMap
    unsigned char _pad064[0xb4 - 0x64];
    CCharString   CurrentFileName;                      // +0xb4
protected:                                              // Ego_r.pdb / FableWin.pdb IAE
    void ParseStringForSymbols(const char* text, bool instantiate);   // 0x009ADE90
    void CreateSymbolsFromPathList(const std::vector<CWideString>& paths, bool instantiate);
};

void CDefinitionManager::CreateSymbolsFromPathList(const std::vector<CWideString>& paths, bool instantiate)
{
    CCharString functionName("DefinitionManager : CreateSymbolsFromPathList");
    for (const CWideString* path = paths.begin(); path != paths.end(); ++path) {
        CWideString fileName;
        CFileScanState scan;
        std::vector<CWideString> files;
        if (ScanFirstFile(*path, scan, fileName)) {
            do {
                files.push_back(((CWideString*)&fileName)->ToCharString().GetAsLowerCase().ToWideString());
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
            ParseStringForSymbols(buffer, instantiate);
            delete[] buffer;
        }
    }
    PSymbolMap->UpdateDirty();
    PDefinitionSymbolMap->UpdateDirty();
}