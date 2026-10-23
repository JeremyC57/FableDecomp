// CUserProfileManager::LoadFileList(std::vector<CWideString>& names, std::vector<CDateAndTime>& times) -- list the extensionless files in the profile directory (other than the profile file itself) into names, with each file's modified time into times
// Grade: DIFFER(534v534, /GS) -- same size and code; retail packs the two push_back type-tag bytes into the `more` byte (local 3, 0x250 frame), ours gives them their own dword (0x254 frame), shifting every stack offset by 4
// Leaf: LoadFileList
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004091C0, length 534 bytes, /GS cookie frame (0x250), `ret 8`.
// Callees: vector<CWideString>::erase(first, last) 0x00412450, the 28-byte __copy(first, last, result, random_access_iterator_tag,
//   ptrdiff_t*) 0x0040E350 (__fastcall), CUserProfileManager::GetPathNameFromFileName(const CWideString&, bool) 0x00408120,
//   operator+(const CWideString&, const wchar_t*) 0x0099BF30, FindFirstFile / FindNextFile 0x00999760 / 0x00999850,
//   SplitPath 0x00997620, CWideString::operator!= 0x0099B0F0, GetFileModifiedTime 0x009992A0, the vectors' _M_insert_overflow
//   0x00412330 (CWideString) / 0x004125A0 (CDateAndTime), CWideString() 0x0099AED0 / (const wchar_t*) 0x0099B6B0 / copy
//   0x0099B720 / dtor 0x0099B510, wcscmp 0x00BFE94C. Strings: L"" 0x0122D70C, L"*" 0x0122DE6C.
// Layout: CUserProfileManager ProfileName +0x08 (Ego_r.pdb); find handle 8 bytes, find data 0x20C (as the landed
//   CFrontendGameComponent::GetWorldNames 0x00495890); CDateAndTime 7 longs (as 0x00829D90).
#include <stddef.h>
#include <new>
extern "C" int __cdecl wcscmp(const wchar_t* a, const wchar_t* b);
struct CWideStringData {
    wchar_t* Begin;
    wchar_t* End;
};
struct CWideString {
    CWideStringData* PStringData;
    CWideString();                              // 0x0099AED0
    CWideString(const wchar_t* text);           // 0x0099B6B0
    CWideString(const CWideString& other);      // 0x0099B720
    ~CWideString();                             // 0x0099B510
    bool operator!=(const CWideString& other) const;   // 0x0099B0F0
    __forceinline bool IsEmpty() const
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
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name, CWideString* extension);   // 0x00997620
struct CFindFileData {
    wchar_t       FileName[260];
    unsigned long Attributes;
};
struct CFindFileHandle {
    long _body[2];
};
bool __fastcall FindFirstFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data);   // 0x00999760
bool __fastcall FindNextFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data);    // 0x00999850
struct CDateAndTime {
    long Fields[7];
    CDateAndTime(long year = 0, long month = 0, long day = 0, long hour = 0, long minute = 0, long second = 0,
                 long millisecond = 0);                 // 0x009E9A70
};
CDateAndTime __fastcall GetFileModifiedTime(const CWideString& path);    // 0x009992A0

namespace std {
struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };
CDateAndTime* __fastcall __copy(CDateAndTime* first, CDateAndTime* last, CDateAndTime* result, const random_access_iterator_tag&, int*);   // 0x0040E350
template <class T, class Trivial> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void _M_insert_overflow(T* pos, const T& x, const Trivial&, unsigned int fillLen, bool atEnd);
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, Trivial(), 1, true);
    }
};
class CWideStringVector : public vector<CWideString, __false_type> {
public:
    CWideString* erase(CWideString* first, CWideString* last);   // 0x00412450
    void clear() { erase(begin(), end()); }
};
class CDateAndTimeVector : public vector<CDateAndTime, __true_type> {
public:
    CDateAndTime* erase(CDateAndTime* first, CDateAndTime* last)
    {
        CDateAndTime* i = __copy(last, _M_finish, first, random_access_iterator_tag(), (int*)0);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
}

class CUserProfileManager {
public:
    unsigned char _pad00[8];
    CWideString   ProfileName;                  // +0x08
    CWideString GetPathNameFromFileName(const CWideString& fileName, bool create) const;   // 0x00408120
    void LoadFileList(std::CWideStringVector& names, std::CDateAndTimeVector& times);
};

void CUserProfileManager::LoadFileList(std::CWideStringVector& names, std::CDateAndTimeVector& times)
{
    names.clear();
    times.clear();
    CWideString pattern = GetPathNameFromFileName(L"", false) + L"*";
    CFindFileHandle handle;
    CFindFileData data;
    bool more = FindFirstFile(pattern, handle, data);
    while (more) {
        CWideString name;
        CWideString extension;
        SplitPath(data.FileName, 0, 0, &name, &extension);
        if (extension.IsEmpty() && name != ProfileName) {
            names.push_back(name);
            CWideString path(data.FileName);
            times.push_back(GetFileModifiedTime(path));
        }
        more = FindNextFile(pattern, handle, data);
    }
}
