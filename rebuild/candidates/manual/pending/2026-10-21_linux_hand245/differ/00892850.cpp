// CGameScriptInterface::AddScreenMessage(const CCharString& textTag, ETextGroupSelectionMethod method) const -- look the tag up in the game text bank (by CRC): a text entry's string is shown as a screen message; for a group entry only the random pick is drawn; an unknown tag shows "(" + tag
// Grade: DIFFER(335v384) -- first-pass model: the CRC key operands and the two counted-pointer releases (retail `mov esi,[eax+4] ; dec esi ; mov [eax],esi` form, a frame-held text-entry pointer with the GetText result in the dead method slot) still differ
// Leaf: AddScreenMessage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00892850, length 383 bytes (`ret 8`).
// Callees: CWideString ctor/dtor/operator=/operator!= @ 0x0099AED0 / 0x0099B510 / 0x0099B7D0 / 0x0099B150, CCRC::Calc
//   @ 0x004014A0 (`__fastcall`), the text bank's CRC lookup @ 0x009CC410, CBankFile::GetEntryDataType @ 0x009CC210,
//   NGameText::CDataBank::GetPTextEntry / GetPGroupEntry (intrusive counted pointers by value) @ 0x009C9070 / 0x009C8890,
//   the text entry's text @ 0x009C7A40, the world's random seed @ 0x0049D810, GFRandom @ 0x004CDAE0, CCharString::ToWideString
//   @ 0x0099E480, operator+(const wchar_t*, const CWideString&) @ 0x0099BFF0, CPlayerGui::AddScreenMessage @ 0x00438AA0.
// Globals: the engine 0x013B86A0 (text bank +0x14), the player GUI 0x013B8790.
typedef unsigned short wchar_fable;
struct CWideString {
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    bool operator!=(const wchar_fable* text) const;     // 0x0099B150
};
CWideString __fastcall operator+(const wchar_fable* left, const CWideString& right);   // 0x0099BFF0
struct CCharStringData { char* PData; unsigned long StringLength; };
struct CCharString {
    CCharStringData* PStringData;
    const char* GetData() const { return PStringData ? PStringData->PData : ""; }
    unsigned long Length() const { return PStringData ? PStringData->StringLength : 0; }
    CWideString ToWideString() const;                   // 0x0099E480
};
struct CCRC { static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0
struct CRefCountedEntry {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
template<class T> struct CIVCountedPointer {
    T* Data;
    ~CIVCountedPointer() { if (Data && --Data->RefCount == 0) Data->Destroy(); }
};
struct CTextEntry : CRefCountedEntry { CWideString GetText() const; };          // 0x009C7A40
struct CGroupEntry : CRefCountedEntry {
    unsigned char _pad08[0x14];
    long*         EntriesStart;                         // +0x1C
    long*         EntriesFinish;                        // +0x20
};
struct CTextDataBank {
    long LookupValue(unsigned long crc) const;          // 0x009CC410
    unsigned long GetEntryDataType(long entry) const;   // 0x009CC210
    CIVCountedPointer<CTextEntry> GetPTextEntry(long entry, bool b) const;     // 0x009C9070
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(long entry, bool b) const;   // 0x009C8890
};
struct CEngine_Retail { unsigned char _pad[0x14]; CTextDataBank* TextBank; };
extern CEngine_Retail* g_pEngine;                       // 0x013B86A0
struct CPlayerGui { void AddScreenMessage(const CWideString& text); };   // 0x00438AA0
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
struct CWorld_Retail { unsigned long* GetRandomSeed(); };   // 0x0049D810
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECT_FIRST, TEXT_GROUP_SELECT_RANDOM };
struct CGameScriptInterface {
    virtual void AddScreenMessage(const CCharString& textTag, ETextGroupSelectionMethod method) const;
    CWorld_Retail* World;                               // +0x04
};

void CGameScriptInterface::AddScreenMessage(const CCharString& textTag, ETextGroupSelectionMethod method) const
{
    CWideString text;
    CTextDataBank* bank = g_pEngine->TextBank;
    long entry = bank->LookupValue(CCRC::Calc(0, textTag.GetData(), textTag.Length()));
    if (entry) {
        if (bank->GetEntryDataType(entry) == 0) {
            text = bank->GetPTextEntry(entry, true).Data->GetText();
        } else {
            CGroupEntry* group;
            {
                CIVCountedPointer<CGroupEntry> pointer = bank->GetPGroupEntry(entry, true);
                group = pointer.Data;
                pointer.Data = 0;
            }
            unsigned long count = group->EntriesFinish - group->EntriesStart;
            if (count > 1 && method)
                GFRandom(count, World->GetRandomSeed());
        }
    } else
        text = L"(" + textTag.ToWideString();
    if (text != L"")
        g_pPlayerGui->AddScreenMessage(text);
}
