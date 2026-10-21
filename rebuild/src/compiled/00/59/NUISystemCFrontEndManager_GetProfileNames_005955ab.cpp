// NUISystem::CFrontEndManager::GetProfileNames(std::vector<CWideString>& names) -- list the profiles: every sub-directory entry of the save directory (skipping "." / ".." and only while the search pattern is not a file) that holds *.bin files contributes its name once per file found
// Leaf: ?GetProfileNames@CFrontEndManager@NUISystem@@
// Flags: /GS
// Retail: Fable.exe @ 0x005955AB, 556 bytes, `ret 4`, /GS cookie (0x0139C8A8, __security_check_cookie 0x00BFE9F9) in a
// biased frame (`lea ebp,[esp-3D0h]`).
// Body: names.erase(begin, end) (0x00412450); pattern = NResourceDirectoryNames::GetSaveDir() (0x0041A540, __fastcall hidden
// result) + L"*" (operator+ 0x0099BF30); FindFirstFile / FindNextFile (0x00999760 / 0x00999850, __fastcall) over pattern:
// fileName(data name) (0x0099B6B0); skip when fileName.ConvertToCharString() (0x0099B2C0) ends with "." (inline length
// difference + inline strcmp) or IsPathFile(pattern) (0x00999A30); else fileName += L"\\" (0x0099B940), SplitPath(fileName,
// 0, &dir, 0, 0) (0x00997620), files = CWideString(fileName) (copy 0x0099B720) + L"*.bin" and an inner find loop whose
// every hit pushes the OUTER entry's name (SplitPath name part) with the out-of-line push_back (0x004128A0).
// Names: Ego_r.pdb CFrontEndManager member; GetSaveDir / IsPathFile / SplitPath / Find* as the landed
// CUserProfileManager::DeleteProfile (0x00407040) and CFrontendGameComponent::GetWorldNames (0x00495890) models.
// Layout: find handle 8 bytes, find data 0x20C; CCharString data block { char* Text; long Length; }.
// Shape: each loop keeps its own `found` flag assigned by FindNextFile inside the body (the loop locals die after the
// call, `mov bl,al` before the destructor).
// Opt: size TU (`#pragma optimize("s", on)`: the biased /GS frame).
#pragma optimize("s", on)
#include <string.h>
struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    CWideString();                              // 0x0099AED0
    CWideString(const unsigned short* text);    // 0x0099B6B0
    CWideString(const CWideString& other);      // 0x0099B720
    ~CWideString();                             // 0x0099B510
    CWideString& operator+=(const unsigned short* text);   // 0x0099B940
    class CCharString ConvertToCharString() const;          // 0x0099B2C0
};
struct CCharStringData {
    char* Text;
    long  Length;
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    __forceinline long Length() const { return PStringData == 0 ? 0 : PStringData->Length; }
    __forceinline const char* c_str() const { return PStringData == 0 ? "" : PStringData->Text; }
    __forceinline bool EndsWith(const CCharString& suffix) const
    {
        long offset = Length() - suffix.Length();
        if (offset >= 0)
            return strcmp(c_str() + offset, suffix.c_str()) == 0;
        return false;
    }
};
CWideString __fastcall operator+(const CWideString& a, const unsigned short* b);       // 0x0099BF30
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name, CWideString* extension);   // 0x00997620
bool __fastcall IsPathFile(const CWideString& path);   // 0x00999A30
struct CFindFileData {
    unsigned short FileName[260];
    unsigned long  Attributes;
};
struct CFindFileHandle {
    long _body[2];
};
bool __fastcall FindFirstFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data);   // 0x00999760
bool __fastcall FindNextFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data);    // 0x00999850
namespace NResourceDirectoryNames {
CWideString __fastcall GetSaveDir();            // 0x0041A540
}
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);                // 0x00412450 for CWideString
    void push_back(const T& x);                 // 0x004128A0 for CWideString
    void clear() { erase(begin(), end()); }
};
}
namespace NUISystem {
class CFrontEndManager {
public:
    void GetProfileNames(std::vector<CWideString>& names);
};
void CFrontEndManager::GetProfileNames(std::vector<CWideString>& names)
{
    names.clear();
    CWideString pattern = NResourceDirectoryNames::GetSaveDir() + (const unsigned short*)L"*";
    CFindFileHandle handle;
    CFindFileData data;
    bool found = FindFirstFile(pattern, handle, data);
    while (found) {
        CWideString fileName(data.FileName);
        bool dots;
        {
            CCharString dot(".");
            dots = fileName.ConvertToCharString().EndsWith(dot);
        }
        if (!dots && !IsPathFile(pattern)) {
            fileName += (const unsigned short*)L"\\";
            CWideString dir;
            SplitPath(fileName, 0, &dir, 0, 0);
            CWideString files = CWideString(fileName) + (const unsigned short*)L"*.bin";
            CFindFileHandle fileHandle;
            CFindFileData fileData;
            bool fileFound = FindFirstFile(files, fileHandle, fileData);
            while (fileFound) {
                CWideString name;
                {
                    CWideString entry(data.FileName);
                    SplitPath(entry, 0, 0, &name, 0);
                }
                names.push_back(name);
                fileFound = FindNextFile(files, fileHandle, fileData);
            }
        }
        found = FindNextFile(pattern, handle, data);
    }
}
}