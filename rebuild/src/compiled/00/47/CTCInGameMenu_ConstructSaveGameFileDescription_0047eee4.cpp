// CTCInGameMenu::ConstructSaveGameFileDescription(CWideString& description, long index) -- build the load/save screen text for save slot `index`: when the save (and its "." companion file, if present) verify, "<profile name label>:<profile name>\n<date><separator><time>\n<region label>:<region display name>\n<time spent in game>"; otherwise the TEXT_GUI_MENU_INVALID_SAVE text with the file name substituted
// Leaf: ?ConstructSaveGameFileDescription@CTCInGameMenu
// Flags: /GS
//
// Retail 0x0047EEE4, 1067 bytes (`ret 8`; biased /GS frame `lea ebp,[esp-1ACh]`, cookie at [ebp+1A8h]). Size TU with only
// `#pragma optimize("s", on)` (GOTCHAS: that alone gives the biased frame).
// Callees: CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0, GetPathNameFromFileName 0x00408120, operator+(const CWideString&,
//   const wchar_t*) 0x0099BF30 (__fastcall, hidden result in ecx), ~CWideString 0x0099B510, PathExists 0x00999230 (__fastcall),
//   CTCInGameMenu::VerifySignatureCached 0x0047ED0A, CManager::GetInstance 0x0041E5F2 and its vslot 42 (+0xA8), CCharString
//   (const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CDataBank::GetTextBySymbol 0x009C95E0 (text bank at engine +0x14,
//   0x013B86A0), CWideString::operator= 0x0099B7D0, operator+=(const CWideString&) 0x0099B8D0, operator+=(const wchar_t*)
//   0x0099B940, CDateAndTime::ToFileTime 0x009E9D40, the date/time formatter 0x00A0D980 (__fastcall), CWideString(const wchar_t*)
//   0x0099B6B0, CWideString() 0x0099AED0, CCharString CRC 0x00404310, the bank's LookupValue 0x009CC410 / GetEntryDataType
//   0x009CC210 / group getters 0x009C8890 and 0x009C9070, the group's GetBankHandle 0x009C7A40, CCharString::ToWideString
//   0x0099E480, std::bitset<>::test 0x00410DE0, the time-spent formatter 0x005CCC91 (__fastcall), the file-name substitution
//   0x009C7980 (__fastcall).
// Layout: retail CTCInGameMenu (12-byte containers, landed ctor 0x0047DF8A): ParentThing +0x04, FileNames +0x28, FileTimes
//   +0x34 (0x1C each), FileRegionNames +0x78, FilePlayTimes +0x90; CUserProfileManager profile name +0x08; CManager slot-42
//   def: separator +0x3C; text group: entry ids +0x1C.
// Shape: the hero-stats interface fetch before the static GetTimeSpentInGame is dead except for the out-of-line bitset test;
//   its pure-load lookup is modelled as an indexed read (any pure load keeps retail's `mov eax,[esi+4] ; lea ecx,[eax+20h]`).
//   The two region arms are one statement each so VC7.1 cross-jumps the shared GetEntry tail.
#pragma optimize("s", on)
#include <stddef.h>

class CWideString { public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const wchar_t* text);                   // 0x0099B6B0
    CWideString(const CWideString& other);
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    CWideString& operator+=(const CWideString& other);  // 0x0099B8D0
    CWideString& operator+=(const wchar_t* text);       // 0x0099B940
};
CWideString __fastcall operator+(const CWideString& s, const wchar_t* text);   // 0x0099BF30
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    unsigned long ComputeCRC32() const;                 // 0x00404310
    CWideString ToWideString() const;                   // 0x0099E480
};
bool __fastcall PathExists(const CWideString& path);   // 0x00999230
struct _FILETIME { unsigned long dwLowDateTime, dwHighDateTime; };
class CDateAndTime { public:
    long _body[7];
    _FILETIME ToFileTime() const;                       // 0x009E9D40
};
void __fastcall FormatDateTime(const _FILETIME& time, wchar_t* dateText, wchar_t* timeText, long flags);   // 0x00A0D980
class CTCHeroStats { public:
    static CWideString __fastcall GetTimeSpentInGame(long seconds, bool brief);    // 0x005CCC91
};
void __fastcall SubstituteFileName(CWideString& text, const CWideString& name); // 0x009C7980

template<class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned long i) { return *(begin() + i); }
    const T* begin() const { return _M_start; }
    const T& operator[](unsigned long i) const { return *(begin() + i); }
};
struct CRefCounted {
    virtual void v00();
    virtual void Destroy();                             // slot 1 (+0x04)
    long m_count;                                       // +0x04
    __forceinline void DecRef() { --m_count; if (m_count == 0) Destroy(); }
};
template<class T> struct CDefPtr {                      // the getters' by-value result
    T* m_p;
    CDefPtr(const CDefPtr& other);
    __forceinline ~CDefPtr() { if (m_p) m_p->DecRef(); }
    T* operator->() const { return m_p; }
};
template<class T> struct CTempPtr {                     // released and cleared
    T* m_p;
    CTempPtr(const CTempPtr& other);
    __forceinline ~CTempPtr() { if (m_p) { m_p->DecRef(); m_p = 0; } }
    T* operator->() const { return m_p; }
};
struct CUIDef : CRefCounted {
    long _pad08[(0x3c - 0x08) / 4];
    CWideString DateTimeSeparator;                      // +0x3C
};
struct CTextGroup : CRefCounted {
    long _pad08[(0x1c - 0x08) / 4];
    CRetailVector<unsigned long> Ids;                   // +0x1C
};
struct CTextEntry : CRefCounted {
    CWideString GetBankHandle() const;                  // 0x009C7A40
};
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);           // 0x009C95E0
    unsigned long LookupValue(unsigned long crc);                     // 0x009CC410
    long GetEntryDataType(unsigned long entry);                       // 0x009CC210
    CTempPtr<CTextGroup> GetGroup(unsigned long entry, bool load);    // 0x009C8890
    CDefPtr<CTextEntry> GetEntry(unsigned long entry, bool load);     // 0x009C9070
};
class CMainGameComponent { public:
    long       _pad00[0x14 / 4];
    CDataBank* TextBank;                                // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
__forceinline CDataBank* GetTextBank() { return g_pMainGameComponent->TextBank; }
namespace NUISystem {
class CManager { public:
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04();
    virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08(); virtual void m09();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13(); virtual void m14();
    virtual void m15(); virtual void m16(); virtual void m17(); virtual void m18(); virtual void m19();
    virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23(); virtual void m24();
    virtual void m25(); virtual void m26(); virtual void m27(); virtual void m28(); virtual void m29();
    virtual void m30(); virtual void m31(); virtual void m32(); virtual void m33(); virtual void m34();
    virtual void m35(); virtual void m36(); virtual void m37(); virtual void m38(); virtual void m39();
    virtual void m40(); virtual void m41();
    virtual CDefPtr<CUIDef> GetUIDef();                 // slot 42 (+0xA8)
    static CManager* __fastcall GetInstance();          // 0x0041E5F2
};
}
class CUserProfileManager { public:
    long        _pad00[2];
    CWideString ProfileName;                            // +0x08
    CWideString GetPathNameFromFileName(const CWideString& name, bool full);   // 0x00408120
    static CUserProfileManager& __fastcall Get();       // 0x0040D2A0
};
struct CPresenceBits { bool test(long kind); };         // bitset<>::test 0x00410DE0
class CThing { public:
    long          _pad00[0x20 / 4];
    CPresenceBits present;                              // +0x20
    long          _pad21[(0x44 - 0x24) / 4];
    class CTCBase** m_firstTC;                         // +0x44 (interface map, simplified)
    template<class T> __forceinline T* GetTC(long kind) { if (present.test(kind)) return (T*)m_firstTC[kind]; return 0; }
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
};
class CTCInGameMenu : public CTCBase {
public:
    long                          _pad0c[(0x28 - 0x0c) / 4];
    CRetailVector<CWideString>    FileNames;            // +0x28
    CRetailVector<CDateAndTime>   FileTimes;            // +0x34
    long                          _pad40[(0x78 - 0x40) / 4];
    CRetailVector<CCharString>    FileRegionNames;      // +0x78
    CRetailVector<CCharString>    FileRegionMiniMapNames;   // +0x84
    CRetailVector<long>           FilePlayTimes;        // +0x90
    bool VerifySignatureCached(const CWideString& path);    // 0x0047ED0A
    void ConstructSaveGameFileDescription(CWideString& description, long index);
};

using namespace NUISystem;

void CTCInGameMenu::ConstructSaveGameFileDescription(CWideString& description, long index)
{
    CWideString companion = CUserProfileManager::Get().GetPathNameFromFileName(FileNames[index], false) + L".";
    bool hasCompanion = false;
    CManager* manager = 0;
    if (PathExists(companion))
        hasCompanion = true;
    bool valid = VerifySignatureCached(CUserProfileManager::Get().GetPathNameFromFileName(FileNames[index], false)) &&
                 (!hasCompanion || VerifySignatureCached(companion));
    if (valid) {
        manager = CManager::GetInstance();
        description = GetTextBank()->GetTextBySymbol("TEXT_GUI_MENU_PROFILE_NAME") + L":";
        description += CUserProfileManager::Get().ProfileName;
        description += L"\n";
        wchar_t timeText[128];
        wchar_t dateText[128];
        FormatDateTime(FileTimes[index].ToFileTime(), dateText, timeText, 0);
        CWideString when(dateText);
        when += manager->GetUIDef()->DateTimeSeparator;
        when += CWideString(timeText);
        description += when;
        description += L"\n";
        description += GetTextBank()->GetTextBySymbol("TEXT_GUI_MENU_REGION") + L":";
        CWideString region;
        CDataBank* bank = GetTextBank();
        unsigned long entry = bank->LookupValue(FileRegionNames[index].ComputeCRC32());
        if (entry > 0) {
            if (bank->GetEntryDataType(entry)) {
                CRetailVector<unsigned long> const& ids = bank->GetGroup(entry, true)->Ids;
                region = bank->GetEntry(ids[0], true)->GetBankHandle();
            } else
                region = bank->GetEntry(entry, true)->GetBankHandle();
        } else
            region = FileRegionNames[index].ToWideString();
        description += region;
        description += L"\n";
        CTCHeroStats* stats = ParentThing->GetTC<CTCHeroStats>(0x14);
        description += stats->GetTimeSpentInGame(FilePlayTimes[index], false);
    } else {
        description = GetTextBank()->GetTextBySymbol("TEXT_GUI_MENU_INVALID_SAVE");
        SubstituteFileName(description, FileNames[index]);
    }
}