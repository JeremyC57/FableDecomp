#include <cstdio>
#include <stdlib.h>
class CWideString;
struct CFileListInfo {                                   // 28-byte element filled by LoadFileList (type not identified)
    unsigned long Data[7];
};
namespace std {
template <class T> class vector {                        // STLport vector<T>
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);                         // 0x00412450 for CWideString
};
}
template <class T> class CMallocVector {                  // STLport vector<T> over the malloc allocator
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CMallocVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CMallocVector() { if (_M_start) free(_M_start); }
};
class CUserProfileManager {
public:
    void LoadFileList(std::vector<CWideString>& names, CMallocVector<CFileListInfo>& infos);   // 0x004091C0
public:
    static CUserProfileManager& __fastcall Get();   // 0x0040D2A0
};
template <class T> class CTBaseSingleton {
public:
    static T& Get();                                     // 0x0040D2A0
};
class CFrontendProfileList {
public:
    std::vector<CWideString> Names;                      // +0x00
    void Refresh();
};
void CFrontendProfileList::Refresh()
{
    Names.erase(Names.begin(), Names.end());
    CMallocVector<CFileListInfo> infos;
    CUserProfileManager::Get().LoadFileList(Names, infos);
}

class CWideString { public: int V; };
static int g_erase, g_load;
static void* g_infos_seen;
template <> CWideString* std::vector<CWideString>::erase(CWideString* first, CWideString* last) { ++g_erase; _M_finish = first; return first; }
static CUserProfileManager g_upm;
CUserProfileManager& CUserProfileManager::Get() { return g_upm; }
void CUserProfileManager::LoadFileList(std::vector<CWideString>& names, CMallocVector<CFileListInfo>& infos)
{
    ++g_load;
    bool empty_infos = infos._M_start == 0 && infos._M_finish == 0 && infos._M_end_of_storage == 0;
    if (!empty_infos || names._M_finish != names._M_start) return;
    names._M_finish = names._M_start + 2;                // two names found
    infos._M_start = (CFileListInfo*)malloc(2 * sizeof(CFileListInfo));
    infos._M_finish = infos._M_end_of_storage = infos._M_start + 2;
    g_infos_seen = infos._M_start;
}
int main()
{
    static CWideString storage[4];
    CFrontendProfileList l; l.Names._M_start = storage; l.Names._M_finish = storage + 3; l.Names._M_end_of_storage = storage + 4;
    l.Refresh();
    bool ok = g_erase == 1 && g_load == 1 && l.Names._M_finish == storage + 2 && g_infos_seen != 0 && sizeof(CFileListInfo) == 0x1c;
    std::printf(ok ? "00495830_TEST PASS\n" : "00495830_TEST FAIL\n");
    return ok ? 0 : 1;
}