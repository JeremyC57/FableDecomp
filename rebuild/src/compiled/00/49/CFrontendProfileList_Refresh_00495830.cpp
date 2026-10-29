// CFrontendProfileList::Refresh() -- clear the name list and refill it from the user-profile manager's file list (the per-file info list is discarded)
// Leaf: ?Refresh@CFrontendProfileList@@QAEXXZ
// Retail: Fable.exe @ 0x00495830, 84 bytes, plain `ret`; one caller 0x0049602B (CFrontendGameComponent, `this` = its
// SaveNames list at +0x1e1c). Catalog row label CEngineLandscapeMap::RenderBackground is wrong; the class name follows
// the existing bake (CFrontendProfileList_Refresh) -- no PDB symbol (the Ego_r.pdb SaveNames is a plain
// vector<CWideString>), so the owning class is not confirmed.
// Body: Names.erase(begin, end) (0x00412450, vector<CWideString>::erase out of line); a local vector of 28-byte file
// infos (zeroed); CTBaseSingleton<CUserProfileManager>::Get() (0x0040D2A0) ->LoadFileList(Names, infos) (0x004091C0);
// the local's destructor frees its storage (free 0x00BFEA14: malloc allocator, trivially destructible elements).
// Layout: Names at +0 (12-byte STLport vector); file-info element 0x1c bytes (LoadFileList grows it by 0x1c), type not
// identified.
// Shape: /O2 (16-aligned entry).
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