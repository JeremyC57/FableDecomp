// CTCInGameMenu::ConstructSaveGameFileDescription(CWideString& description, long index) -- build the load/save screen text for save slot `index`: when the save (and its "." companion file, if present) verify, "<profile name label>:<profile name>\n<date><separator><time>\n<region label>:<region display name>\n<time spent in game>"; otherwise the TEXT_GUI_MENU_INVALID_SAVE text with the file name substituted
// Leaf: ?ConstructSaveGameFileDescription@CTCInGameMenu@@QAEXAAVCWideString@@J@Z
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
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
// NF72 (round 7, q10909): ported onto the shared headers. CWideString / CCharString come from fable_string.h (retail branches); the
//   text bank, its text / group entries and the counted pointers from engine/NGameText_CDataBank.h (CGroupEntry : CResource, the
//   DATABANK_GROUPTEXT + GETPGROUPENTRY branch agentNF69 proved on 0x009C8890; CArray<long> Entries at +0x1C); the UI manager from
//   engine/NUISystem_CManager.h (slot 42 = GetMiscThingsDef, by-value CDefPointer<CUIMiscThingsDef>; the separator is
//   CUIMiscThingsDef::SpaceSeparator +0x3C, engine/NUISystem_CUIMiscThingsDef.h); the bitset from stlport/fable_containers.h
//   (FABLE_RETAIL_STLP_BITSET_TEST_DECL). The file-name substitution 0x009C7980 is NGameText::ConstructString(CWideString&, const
//   CWideString&) (Ego_r.pdb 0x00B156F0 + FableWin.pdb 0x02F6E6F0, __fastcall, returns bool; FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING); the
//   landed made-up SubstituteFileName is dropped. The local CRefCounted / CDefPtr / CIVCountedPointer / CRetailVector / CGroupEntry /
//   CTextEntry / CDataBank / CManager / CUIDef / CWideString / CCharString / bitset models are gone; CAFile, CDateAndTime, CTCHeroStats,
//   CUserProfileManager, CMainGameComponent, CThing, CTCBase and CTCInGameMenu stay local (no change asked here).
#include "fable_compiler.h"
#pragma optimize("s", on)
#include <stddef.h>
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT_CLEAR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
#define FABLE_RETAIL_NGAMETEXT_GETPGROUPENTRY
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCELIST_MAKEFREESPACE
#define FABLE_RETAIL_C3DANIMATIONINFO_BUILD
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CManager.h"

class CAFile { public: static bool __fastcall PathExists(const CWideString& path); };   // 0x00999230
struct _FILETIME { unsigned long dwLowDateTime, dwHighDateTime; };
class CDateAndTime { public:
    long _body[7];
    _FILETIME ToFileTime() const;                       // 0x009E9D40
};
void __fastcall FormatDateTime(const _FILETIME& time, wchar_t* dateText, wchar_t* timeText, long flags);   // 0x00A0D980
class CTCHeroStats { public:
    static CWideString __fastcall GetTimeSpentInGame(long seconds, bool brief);    // 0x005CCC91
};

template<class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned long i) { return *(begin() + i); }
    const T* begin() const { return _M_start; }
    const T& operator[](unsigned long i) const { return *(begin() + i); }
};
using NGameText::CDataBank;
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    long       _pad00[0x14 / 4];
    CDataBank* TextBank;                                // +0x14
};
FABLE_FORCEINLINE CDataBank* GetTextBank() { return CMainGameComponent::Get()->TextBank; }
class CUserProfileManager { public:
    long        _pad00[2];
    CWideString ProfileName;                            // +0x08
    CWideString GetPathNameFromFileName(const CWideString& name, bool full) const;   // 0x00408120
    static CUserProfileManager& __fastcall Get();       // 0x0040D2A0
};
class CThing { public:
    long          _pad00[0x20 / 4];
    std::bitset<274> present;                              // +0x20
    long          _pad21[(0x44 - 0x24) / 4];
    class CTCBase** m_firstTC;                         // +0x44 (interface map, simplified)
    template<class T> FABLE_FORCEINLINE T* GetTC(long kind) { if (present.test(kind)) return (T*)m_firstTC[kind]; return 0; }
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
    if (CAFile::PathExists(companion))
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
        when += manager->GetMiscThingsDef()->SpaceSeparator;
        when += CWideString(timeText);
        description += when;
        description += L"\n";
        description += GetTextBank()->GetTextBySymbol("TEXT_GUI_MENU_REGION") + L":";
        CWideString region;
        CDataBank* bank = GetTextBank();
        unsigned long entry = bank->FindIndexBySymbolCRC(FileRegionNames[index].GetCRC());
        if (entry > 0) {
            if (bank->GetEntryDataType(entry)) {
                const CArray<long>& ids = bank->GetPGroupEntry(entry, true)->Entries;
                region = bank->GetPTextEntry(ids[0], true)->GetText();
            } else
                region = bank->GetPTextEntry(entry, true)->GetText();
        } else
            region = FileRegionNames[index].ToWideString();
        description += region;
        description += L"\n";
        CTCHeroStats* stats = ParentThing->GetTC<CTCHeroStats>(0x14);
        description += stats->GetTimeSpentInGame(FilePlayTimes[index], false);
    } else {
        description = GetTextBank()->GetTextBySymbol("TEXT_GUI_MENU_INVALID_SAVE");
        NGameText::ConstructString(description, FileNames[index]);
    }
}
