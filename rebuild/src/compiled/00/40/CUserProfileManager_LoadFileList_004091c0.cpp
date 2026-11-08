// CUserProfileManager::LoadFileList(std::vector<CWideString>& names, std::vector<CDateAndTime>& times) -- list the extensionless files in the profile directory (other than the profile file itself) into names, with each file's modified time into times
// Leaf: LoadFileList
// Naming wave 28 (family d3): the vectors are STLport vector<T, allocator<T> > (DECISIONS_W26 D7 / W27 D3): erase
//   0x00412450 ?erase@?$vector@VCWideString@@V?$allocator@..@std@@@std@@QAEPAVCWideString@@PAV3@0@Z, the grow-inserts 0x00412330 /
//   0x004125A0 the protected __false_type members (IAE), the inline CDateAndTime erase's __copy 0x0040E350 the template
//   ??$__copy@PAVCDateAndTime@@PAV1@H@std@@YI..@Z (0x0059AEAF's spelling; 0x0040E350 itself is a folded copy, held). Was
//   CWideStringVector / CDateAndTimeVector stand-ins with a same-TU member-template grow-insert body.
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004091C0, length 534 bytes, /GS cookie frame (0x250), `ret 8`.
// Callees: vector<CWideString>::erase(first, last) 0x00412450, the 28-byte __copy(first, last, result, random_access_iterator_tag,
//   ptrdiff_t*) 0x0040E350 (__fastcall), CUserProfileManager::GetPathNameFromFileName(const CWideString&, bool) 0x00408120,
//   operator+(const CWideString&, const wchar_t*) 0x0099BF30, CAFile::StartFileScan / ScanNextFile 0x00999760 / 0x00999850,
//   SplitPath 0x00997620, CWideString::operator!= 0x0099B0F0, GetFileModifiedTime 0x009992A0, the vectors' _M_insert_overflow
//   0x00412330 (CWideString) / 0x004125A0 (CDateAndTime), CWideString() 0x0099AED0 / (const wchar_t*) 0x0099B6B0 / copy
//   0x0099B720 / dtor 0x0099B510, wcscmp 0x00BFE94C. Strings: L"" 0x0122D70C, L"*" 0x0122DE6C.
// Layout: CUserProfileManager ProfileName +0x08 (Ego_r.pdb); find handle 8 bytes, find data 0x20C (as the landed
//   CFrontendGameComponent::GetWorldNames 0x00495890); CDateAndTime 7 longs (as 0x00829D90).
#include "fable_compiler.h"
#include <stddef.h>
#include <new>
void StandInSink(void* p, unsigned int n);
extern "C" int __cdecl wcscmp(const wchar_t* a, const wchar_t* b);
class CWideStringData { public:
    wchar_t* Begin;
    wchar_t* End;
};
class CWideString { public:
    CWideStringData* PStringData;
    CWideString();                              // 0x0099AED0
    CWideString(const wchar_t* text);           // 0x0099B6B0
    CWideString(const CWideString& other);      // 0x0099B720
    ~CWideString();                             // 0x0099B510
    bool operator!=(const CWideString& other) const;   // 0x0099B0F0
    FABLE_FORCEINLINE bool IsEmpty() const
    {
        if (PStringData == 0) {
            if (wcscmp(L"", L"") == 0)
                return true;
            return false;
        }
        if (PStringData->End - PStringData->Begin == 0)
            return true;
        return false;
    }
};
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b);       // 0x0099BF30
class CAFile {
public:
    class CFileScanInfo {
    public:
        bool Stopped;                                   // +0x00
        long SearchHandle;                              // +0x04
    };
    static void __fastcall SplitPath(const CWideString&, CWideString*, CWideString*, CWideString*, CWideString*);   // 0x00997620
    static bool __fastcall StartFileScan(const CWideString& pattern, CFileScanInfo& info, wchar_t* firstFile);   // 0x00999760
    static bool __fastcall ScanNextFile(const CWideString& pattern, CFileScanInfo& info, wchar_t* nextFile);     // 0x00999850
};   // 0x00997620
struct CFindFileData {
    wchar_t       FileName[260];
    unsigned long Attributes;
};
class CDateAndTime { public:
    long Fields[7];
    CDateAndTime(long year = 0, long month = 0, long day = 0, long hour = 0, long minute = 0, long second = 0,
                 long millisecond = 0);                 // 0x009E9A70
};
CDateAndTime __fastcall GetFileModifiedTime(const CWideString& path);    // 0x009992A0

namespace std {
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);   // 0x0040E350 (<CDateAndTime*, CDateAndTime*, int>)
template <class _Tp> class allocator { public: allocator() {} };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport 4.6 vector, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
protected:
    void _M_insert_overflow(_Tp* pos, const _Tp& x, const __false_type&, unsigned int fillLen, bool atEnd);   // 0x00412330 / 0x004125A0
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
    _Tp* erase(_Tp* first, _Tp* last)
    {
        _Tp* i = __copy(last, _M_finish, first, random_access_iterator_tag(), (int*)0);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
// vector<CWideString>::erase is the out-of-line instantiation 0x00412450 (retail did not inline it here)
template <> CWideString* vector<CWideString>::erase(CWideString* first, CWideString* last);
// Same-TU stand-in body of the two grow-inserts (retail calls them out of line): VC7.1 then sees the tag temporaries'
// addresses do not escape and shares their frame slot (an external declaration costs 4 frame bytes, 0x254 v 0x250).
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* pos, const _Tp& x, const __false_type&, unsigned int fillLen, bool atEnd)
{
    StandInSink(pos, fillLen);
}
}

class CUserProfileManager {
public:
    unsigned char _pad00[8];
    CWideString   ProfileName;                  // +0x08
    CWideString GetPathNameFromFileName(const CWideString& fileName, bool create) const;   // 0x00408120
    void LoadFileList(std::vector<CWideString>& names, std::vector<CDateAndTime>& times);
};

void CUserProfileManager::LoadFileList(std::vector<CWideString>& names, std::vector<CDateAndTime>& times)
{
    names.clear();
    times.clear();
    CWideString pattern = GetPathNameFromFileName(L"", false) + L"*";
    CAFile::CFileScanInfo handle;
    CFindFileData data;
    bool more = CAFile::StartFileScan(pattern, handle, data.FileName);
    while (more) {
        CWideString name;
        CWideString extension;
        CAFile::SplitPath(data.FileName, 0, 0, &name, &extension);
        if (extension.IsEmpty() && name != ProfileName) {
            names.push_back(name);
            CWideString path(data.FileName);
            times.push_back(GetFileModifiedTime(path));
        }
        more = CAFile::ScanNextFile(pattern, handle, data.FileName);
    }
}