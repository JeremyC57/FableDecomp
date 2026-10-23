// CTCInGameMenu::GetFileNameList() -- rebuild the save-file lists: clear the names, times, region names, minimap names and play times, reload the names/times from the profile manager, then either (slot mode) fill each listed file's time and region info (empty names become the empty-slot name) plus one trailing empty slot, or (fixed mode) put the autosave first when it is listed on the load/save screens and fill one entry per manual save slot
// Grade: DIFFER(1626v1627) -- one residue: the count sum (retail `mov ecx,[first] ; lea ebx,[eax+ecx]`, ours `mov ebx,eax ; add ebx,[first]`; tried named manual count, +=, operand swap, unsigned, ternary). The manual-slot loop matches only with an explicit guard + do-while and a named byte offset for FileTimes (the advisor's user-variable recipe); a plain for loop leaves slot=0/tail-order scheduling residue
// Leaf: ?GetFileNameList@CTCInGameMenu
//
// Retail 0x0047FE83, 1627 bytes (plain `ret`; biased frame `lea ebp,[esp-78h]`). Size TU with only `#pragma optimize("s", on)`.
// Callees: the five vectors' erase(first, last) 0x00412450 / 0x00411B00 / 0x0043336A (x2) / 0x00440590, push_back 0x004128A0 /
//   0x00412970 / 0x0044BFF0 / 0x00485FF3, resize 0x00412B70 / 0x0048623F; CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0,
//   LoadFileList 0x004091C0, the profile manager's bool query 0x00407030, GetEmptySlotName 0x00406A80, GetAutoSaveFileName
//   0x00406690, the manual-save count 0x004067B0, the manual-save slot name 0x00407360, GetPathNameFromFileName 0x00408120;
//   CWideString operator!=(const wchar_t*) 0x0099B150 / operator= 0x0099B7D0 / ConvertToCharString 0x0099B2C0 / ~ 0x0099B510;
//   CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0; _stricmp 0x00BFEBA8; CDateAndTime() 0x009E9AB0;
//   GetFileModifiedTime 0x009992A0 (__fastcall, hidden result in ecx); CTCInGameMenu::AddRegionAndTimeInfo 0x0047F58B.
// Layout: retail CTCInGameMenu (12-byte containers, landed ctor 0x0047DF8A): ActiveScreen +0x18, FileNames +0x28, FileTimes
//   +0x34 (CDateAndTime 0x1C), FileRegionNames +0x78, FileRegionMiniMapNames +0x84, FilePlayTimes +0x90.
#pragma optimize("s", on)
#include <stddef.h>

extern "C" int __cdecl _stricmp(const char* a, const char* b);
extern const char g_emptyCharString[];                 // 0x0129AAF4 (the inline c_str's "")
struct CCharStringData {
    char* Data;
};
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const char* c_str() const { if (PStringData == 0) return g_emptyCharString; return PStringData->Data; }
};
__forceinline bool SameNameNoCase(const CCharString& a, const CCharString& b) { return _stricmp(a.c_str(), b.c_str()) == 0; }
struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    bool operator!=(const wchar_t* text) const;         // 0x0099B150
    CCharString ConvertToCharString() const;            // 0x0099B2C0
};
struct CDateAndTime {
    long Fields[7];
    CDateAndTime();                                     // 0x009E9AB0
};
CDateAndTime __fastcall GetFileModifiedTime(const CWideString& path);   // 0x009992A0

struct CWideStringVector {
    CWideString* _M_start;
    CWideString* _M_finish;
    CWideString* _M_end_of_storage;
    CWideString* begin() { return _M_start; }
    CWideString* end() { return _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    CWideString& operator[](unsigned long i) { return *(begin() + i); }
    CWideString* erase(CWideString* first, CWideString* last);   // 0x00412450
    void clear() { erase(begin(), end()); }
    void push_back(const CWideString& value);           // 0x004128A0
    void resize(unsigned long count);                   // 0x00412B70
};
struct CDateAndTimeVector {
    CDateAndTime* _M_start;
    CDateAndTime* _M_finish;
    CDateAndTime* _M_end_of_storage;
    CDateAndTime* begin() { return _M_start; }
    CDateAndTime* end() { return _M_finish; }
    CDateAndTime& operator[](unsigned long i) { return *(begin() + i); }
    CDateAndTime* erase(CDateAndTime* first, CDateAndTime* last);   // 0x00411B00
    void clear() { erase(begin(), end()); }
    void push_back(const CDateAndTime& value);          // 0x00412970
    void resize(unsigned long count);                   // 0x0048623F
};
struct CCharStringVector {
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharString* begin() { return _M_start; }
    CCharString* end() { return _M_finish; }
    CCharString* erase(CCharString* first, CCharString* last);   // 0x0043336A
    void clear() { erase(begin(), end()); }
    void push_back(const CCharString& value);           // 0x0044BFF0
};
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() { return _M_start; }
    long* end() { return _M_finish; }
    long* erase(long* first, long* last);               // 0x00440590
    void clear() { erase(begin(), end()); }
    void push_back(const long& value);                  // 0x00485FF3
};

struct CUserProfileManager {
    void LoadFileList(CWideStringVector& names, CDateAndTimeVector& times);   // 0x004091C0
    bool IsQuery_00407030();                            // 0x00407030
    CWideString GetEmptySlotName();                     // 0x00406A80
    CWideString GetAutoSaveFileName();                  // 0x00406690
    long GetManualSaveCount_004067b0();                 // 0x004067B0
    const CWideString& GetManualSaveSlotName_00407360(long slot);   // 0x00407360
    CWideString GetPathNameFromFileName(const CWideString& name, bool full);   // 0x00408120
    static CUserProfileManager& __fastcall Get();       // 0x0040D2A0
};

struct CTCBase {
    virtual void v00();
    struct CThing* ParentThing;                         // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
};
class CTCInGameMenu : public CTCBase {
public:
    long               _pad0c[(0x18 - 0x0c) / 4];
    long               ActiveScreen;                    // +0x18
    long               _pad1c[(0x28 - 0x1c) / 4];
    CWideStringVector  FileNames;                       // +0x28
    CDateAndTimeVector FileTimes;                       // +0x34
    long               _pad40[(0x78 - 0x40) / 4];
    CCharStringVector  FileRegionNames;                 // +0x78
    CCharStringVector  FileRegionMiniMapNames;          // +0x84
    CLongVector        FilePlayTimes;                   // +0x90
    void AddRegionAndTimeInfo(const CWideString& path); // 0x0047F58B
    void GetFileNameList();
};

void CTCInGameMenu::GetFileNameList()
{
    FileNames.clear();
    FileTimes.clear();
    FileRegionNames.clear();
    FileRegionMiniMapNames.clear();
    FilePlayTimes.clear();
    CUserProfileManager::Get().LoadFileList(FileNames, FileTimes);
    if (CUserProfileManager::Get().IsQuery_00407030()) {
        for (long i = 0; i < (long)FileNames.size(); ++i) {
            if (FileNames[i] != L"") {
                CWideString path = CUserProfileManager::Get().GetPathNameFromFileName(FileNames[i], false);
                if (path != L"") {
                    AddRegionAndTimeInfo(path);
                    FileTimes[i] = GetFileModifiedTime(path);
                } else {
                    FileTimes[i] = CDateAndTime();
                    FileRegionNames.push_back("");
                    FileRegionMiniMapNames.push_back("");
                    FilePlayTimes.push_back(0);
                }
            } else {
                FileNames[i] = CUserProfileManager::Get().GetEmptySlotName();
                FileTimes[i] = CDateAndTime();
                FileRegionNames.push_back("");
                FileRegionMiniMapNames.push_back("");
                FilePlayTimes.push_back(0);
            }
        }
        FileNames.push_back(CUserProfileManager::Get().GetEmptySlotName());
        FileTimes.push_back(CDateAndTime());
        FileRegionNames.push_back("");
        FileRegionMiniMapNames.push_back("");
        FilePlayTimes.push_back(0);
        return;
    }
    bool hasAutoSave = false;
    if (ActiveScreen == 3 || ActiveScreen == 0) {
        CWideString autoSave = CUserProfileManager::Get().GetAutoSaveFileName();
        for (CWideString* it = FileNames.begin(); it != FileNames.end(); ++it) {
            bool same = SameNameNoCase(it->ConvertToCharString(), autoSave.ConvertToCharString());
            if (same) {
                hasAutoSave = true;
                break;
            }
        }
    }
    long first = 0;
    if (hasAutoSave)
        first = 1;
    long count = CUserProfileManager::Get().GetManualSaveCount_004067b0() + first;
    FileNames.resize(count);
    FileTimes.resize(count);
    if (hasAutoSave) {
        FileNames[0] = CUserProfileManager::Get().GetAutoSaveFileName();
        CWideString path = CUserProfileManager::Get().GetPathNameFromFileName(CUserProfileManager::Get().GetAutoSaveFileName(), false);
        if (path != L"") {
            FileTimes[0] = GetFileModifiedTime(path);
            AddRegionAndTimeInfo(path);
        } else {
            FileTimes[0] = CDateAndTime();
            FileRegionNames.push_back("");
            FileRegionMiniMapNames.push_back("");
            FilePlayTimes.push_back(0);
        }
    }
    long i = first;
    if (i < count) {
        long timeOffset = i * (long)sizeof(CDateAndTime);
        long slot = 0;
        do {
            FileNames[i] = CUserProfileManager::Get().GetManualSaveSlotName_00407360(slot);
            if (FileNames[i] != L"") {
                CWideString path = CUserProfileManager::Get().GetPathNameFromFileName(FileNames[i], false);
                if (path != L"") {
                    *(CDateAndTime*)((char*)FileTimes.begin() + timeOffset) = GetFileModifiedTime(path);
                    AddRegionAndTimeInfo(path);
                } else {
                    *(CDateAndTime*)((char*)FileTimes.begin() + timeOffset) = CDateAndTime();
                    FileRegionNames.push_back("");
                    FileRegionMiniMapNames.push_back("");
                    FilePlayTimes.push_back(0);
                }
            } else {
                FileNames[i] = CUserProfileManager::Get().GetEmptySlotName();
                *(CDateAndTime*)((char*)FileTimes.begin() + timeOffset) = CDateAndTime();
                FileRegionNames.push_back("");
                FileRegionMiniMapNames.push_back("");
                FilePlayTimes.push_back(0);
            }
            ++i;
            ++slot;
            timeOffset += sizeof(CDateAndTime);
        } while (i < count);
    }
}
