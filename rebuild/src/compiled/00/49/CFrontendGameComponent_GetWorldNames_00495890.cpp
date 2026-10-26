// CFrontendGameComponent::GetWorldNames() -- debug front end mode 10: clear the world selection, list every Data\Levels\*.wld file name (no extension) into WorldNames, sort it and reset the scroll offset
// Leaf: ?GetWorldNames@CFrontendGameComponent@@AAEXXZ (Ego_r.pdb, private; modelled public)
// Flags: /GS
// Retail: Fable.exe @ 0x00495890, 323 bytes, plain `ret`, /GS cookie.
// Body: WorldToLoad = L"" (CWideString::operator=(const wchar_t*) 0x0099B800, L"" 0x0122D70C); WorldNames.erase(begin, end)
// (0x00412450); pattern = GetLevelsDirectory() (0x0041A060, __fastcall hidden result, L"Data\\Levels\\") + L"*.wld"
// (operator+ 0x0099BF30, literal 0x0123877C); FindFirstFile / FindNextFile (0x00999760 / 0x00999850) loop: fileName(data
// name) (0x0099B6B0), name() (0x0099AED0), SplitPath(fileName, 0, 0, &name, 0) (0x00997620), WorldNames.push_back(name)
// (inline STLport: copy ctor 0x0099B720 or _M_insert_overflow 0x00412330); sort(begin, end, less) (0x004977D0, LTCG
// __fastcall); WorldNamesOffset = 0.
// Names: the manifest's CBrushLibraryDialog::FillInListBox / PopulateProfiles labels are stale (GetWorldNames is the
// Ego_r.pdb member; it fills the mode-10 world list); 0x0041A060 builds L"Data\\Levels\\" (labelled
// CUserProfileManager::GetAutoSaveFileName, wrong).
// Layout: retail CFrontendGameComponent WorldNames +0x1E30, WorldNamesOffset +0x1E3C, WorldToLoad +0x1E40 (Ego_r.pdb
// +0x1E38/+0x1E48/+0x1E4C less the STLport vector shrink); find handle 8 bytes, find data 0x20C (as the landed
// CMainGameComponent::GenerateMetFilesFromLugFiles 0x00418C3B).
// Shape: the empty std::less needs a user ctor (else `less<CWideString>()` zero-stores its byte before the sort call).
// Opt: /O2 (16-aligned entry).
#include <stddef.h>
#include <new>
class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    CWideString();                              // 0x0099AED0
    CWideString(const wchar_t* text);           // 0x0099B6B0
    CWideString(const CWideString& other);      // 0x0099B720
    ~CWideString();                             // 0x0099B510
    CWideString& operator=(const wchar_t* text);   // 0x0099B800
};
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b);       // 0x0099BF30
class CAFile {
public:
    static void __fastcall SplitPath(const CWideString&, CWideString*, CWideString*, CWideString*, CWideString*);   // 0x00997620
};   // 0x00997620
struct CFindFileData {
    wchar_t       FileName[260];
    unsigned long Attributes;
};
struct CFindFileHandle {
    long _body[2];
};
bool __fastcall FindFirstFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data);   // 0x00999760
bool __fastcall FindNextFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data);    // 0x00999850
CWideString __fastcall GetLevelsDirectory();    // 0x0041A060 (L"Data\\Levels\\")

namespace std {
struct __false_type { __false_type() {} };
template <class T> struct less { less() {} };
template <class It, class Cmp> void __fastcall sort(It first, It last, Cmp comp);   // 0x004977D0 for CWideString*
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);                         // 0x00412450 for CWideString
    void _M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned int fillLen, bool atEnd);   // 0x00412330
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    void clear() { erase(begin(), end()); }
};
}

class CFrontendGameComponent {
public:
    unsigned char             _pad0000[0x1e30];
    std::vector<CWideString>  WorldNames;               // +0x1e30
    long                      WorldNamesOffset;         // +0x1e3c
    CWideString               WorldToLoad;              // +0x1e40

private:
    void GetWorldNames();
};

void CFrontendGameComponent::GetWorldNames()
{
    WorldToLoad = L"";
    WorldNames.clear();
    CWideString pattern = GetLevelsDirectory() + L"*.wld";
    CFindFileHandle handle;
    CFindFileData data;
    if (FindFirstFile(pattern, handle, data)) {
        do {
            CWideString fileName(data.FileName);
            CWideString name;
            CAFile::SplitPath(fileName, 0, 0, &name, 0);
            WorldNames.push_back(name);
        } while (FindNextFile(pattern, handle, data));
    }
    std::sort(WorldNames.begin(), WorldNames.end(), std::less<CWideString>());
    WorldNamesOffset = 0;
}