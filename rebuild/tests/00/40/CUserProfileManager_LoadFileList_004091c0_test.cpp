// 004091c0 behaviour test: CUserProfileManager::LoadFileList (match.cpp body, host-modelled engine stand-ins)
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <new>
#include <string>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)

class CWideString { public:
    std::wstring S; bool Null;
    CWideString() : Null(true) {}
    CWideString(const wchar_t* t) : S(t), Null(false) {}
    bool operator!=(const CWideString& o) const { return S != o.S; }
    bool IsEmpty() const { return S.empty(); }
};
static CWideString operator+(const CWideString& a, const wchar_t* b) { return CWideString((a.S + b).c_str()); }
// SplitPath: name = text after last '\\' up to the last '.', extension = from the last '.'
static void SplitPath(const CWideString& path, CWideString*, CWideString*, CWideString* name, CWideString* ext)
{
    std::wstring p = path.S; size_t sl = p.rfind(L'\\'); std::wstring f = sl == std::wstring::npos ? p : p.substr(sl + 1);
    size_t dot = f.rfind(L'.');
    *name = CWideString((dot == std::wstring::npos ? f : f.substr(0, dot)).c_str());
    *ext = CWideString(dot == std::wstring::npos ? L"" : f.substr(dot).c_str());
}
struct CFindFileData { wchar_t FileName[260]; unsigned long Attributes; };
struct CFindFileHandle { int Index; };
static const wchar_t* g_files[] = { L"save1", L"Profile", L"notes.txt", L"save2", 0 };
static std::wstring g_pattern;
static bool Fill(CFindFileHandle& h, CFindFileData& d) { if (!g_files[h.Index]) return false; wcscpy(d.FileName, g_files[h.Index]); return true; }
static bool FindFirstFile(const CWideString& pat, CFindFileHandle& h, CFindFileData& d) { g_pattern = pat.S; h.Index = 0; return Fill(h, d); }
static bool FindNextFile(const CWideString&, CFindFileHandle& h, CFindFileData& d) { ++h.Index; return Fill(h, d); }
class CDateAndTime { public: long Fields[7]; };
static CDateAndTime GetFileModifiedTime(const CWideString& path) { CDateAndTime t; memset(&t, 0, sizeof t); t.Fields[0] = (long)path.S.size(); return t; }

class CUserProfileManager { public:
    CWideString ProfileName;
    CWideString GetPathNameFromFileName(const CWideString& f, bool) const { return CWideString((L"C:\\Profiles\\" + f.S).c_str()); }
    void LoadFileList(std::vector<CWideString>& names, std::vector<CDateAndTime>& times)
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
};

int main()
{
    CUserProfileManager m; m.ProfileName = CWideString(L"Profile");
    std::vector<CWideString> names; std::vector<CDateAndTime> times;
    names.push_back(CWideString(L"stale")); CDateAndTime z; memset(&z, 0, sizeof z); times.push_back(z);
    m.LoadFileList(names, times);
    CHECK(g_pattern == L"C:\\Profiles\\*");
    CHECK(names.size() == 2 && times.size() == 2);
    CHECK(names.size() == 2 && names[0].S == L"save1" && names[1].S == L"save2");
    CHECK(times.size() == 2 && times[0].Fields[0] == 5 && times[1].Fields[0] == 5);
    if (!g_fail) printf("004091c0_TEST PASS\n");
    return g_fail;
}