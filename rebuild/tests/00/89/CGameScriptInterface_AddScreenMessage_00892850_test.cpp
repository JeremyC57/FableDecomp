// Behaviour test for 00892850: src/00892850.cpp copied verbatim, every declared callee stubbed below so calls and
// arguments are observable. CGameScriptInterface::AddScreenMessage(const CCharString&, ETextGroupSelectionMethod) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CGameScriptInterface::AddScreenMessage(const CCharString& textTag, ETextGroupSelectionMethod method) const -- look the tag up in the game text bank (by CRC): a text entry's string is shown as a screen message; for a group entry only the random pick is drawn; an unknown tag shows "(" + tag
// Leaf: AddScreenMessage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00892850, length 383 bytes (`ret 8`).
// Callees: CWideString ctor/dtor/operator=/operator!= @ 0x0099AED0 / 0x0099B510 / 0x0099B7D0 / 0x0099B150, CCRC::Calc
//   @ 0x004014A0 (`__fastcall`), the text bank's CRC lookup @ 0x009CC410, CBankFile::GetEntryDataType @ 0x009CC210,
//   NGameText::CDataBank::GetPTextEntry / GetPGroupEntry (intrusive counted pointers by value) @ 0x009C9070 / 0x009C8890,
//   the text entry's text @ 0x009C7A40, the world's random seed @ 0x0049D810, GFRandom @ 0x004CDAE0, CCharString::ToWideString
//   @ 0x0099E480, operator+(const wchar_t*, const CWideString&) @ 0x0099BFF0, CPlayerGui::AddScreenMessage @ 0x00438AA0.
// Globals: the engine 0x013B86A0 (text bank +0x14), the player GUI 0x013B8790.
// Shape: the text and group entries are the landed SetQuestInfoText 0x00891A00 model (CRefVirtual + CRefCounted bases,
//   inline ReleaseTemporary = DecRef / GetRefCount / Destroy, the `mov esi,[eax+4] ; ... ; mov [eax],esi` release);
//   CIVCountedPointer's destructor zeroes Data inside its null test. Both entry pointers are TEMPORARIES of their full
//   expressions: the text path's zero store dies with the temporary, the group path keeps a pointer to the entry list
//   (`&bank->GetPGroupEntry(..)->Entries`, read after the release exactly as retail) and its zero store survives.
typedef unsigned short wchar_fable;
class CWideString { public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    bool operator!=(const wchar_fable* text) const;     // 0x0099B150
};
CWideString __fastcall operator+(const wchar_fable* left, const CWideString& right);   // 0x0099BFF0
class CCharStringData { public: char* PData; unsigned long StringLength; };
class CCharString { public:
    CCharStringData* PStringData;
    const char* GetData() const { return PStringData == 0 ? "" : PStringData->PData; }
    unsigned long Length() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    CWideString ToWideString() const;                   // 0x0099E480
};
struct CCRC { static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CRefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
struct CRefCountedEntry : CRefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template<class T> class CIVCountedPointer { public:
    T* Data;
    ~CIVCountedPointer() { if (Data) { Data->ReleaseTemporary(); Data = 0; } }
    T* operator->() const { return Data; }
};
struct CTextEntry : CRefCountedEntry { CWideString GetText() const; };          // 0x009C7A40
template<class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
};
struct CGroupEntry : CRefCountedEntry {
    unsigned char _pad08[0x14];
    CRetailVector<long> Entries;                        // +0x1C
};
struct CTextDataBank {
    long LookupValue(unsigned long crc) const;          // 0x009CC410
    unsigned long GetEntryDataType(long entry) const;   // 0x009CC210
    CIVCountedPointer<CTextEntry> GetPTextEntry(long entry, bool b) const;     // 0x009C9070
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(long entry, bool b) const;   // 0x009C8890
};
struct CEngine_Retail { unsigned char _pad[0x14]; CTextDataBank* TextBank; };
extern CEngine_Retail* g_pEngine;                       // 0x013B86A0
class CPlayerGui { public: void AddScreenMessage(const CWideString& text); };   // 0x00438AA0
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
struct CWorld_Retail { unsigned long* GetRandomSeed(); };   // 0x0049D810
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECT_FIRST, TEXT_GROUP_SELECT_RANDOM };
class CGameScriptInterface { public:
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
            text = bank->GetPTextEntry(entry, true)->GetText();
        } else {
            const CRetailVector<long>* entries = &bank->GetPGroupEntry(entry, true)->Entries;
            unsigned long count = entries->size();
            if (count > 1 && method)
                GFRandom(count, World->GetRandomSeed());
        }
    } else
        text = L"(" + textTag.ToWideString();
    if (text != L"")
        g_pPlayerGui->AddScreenMessage(text);
}

// ---- stubs ----
// wide strings are modelled by a pointer to a static marker string (0 = empty)
static const wchar_fable kEmpty[1] = { 0 };
static const wchar_fable kHello[3] = { 'h', 'i', 0 };
static const wchar_fable kParen[4] = { '(', 't', 'g', 0 };
static int g_wdtors = 0;
CWideString::CWideString() : PStringData(0) {}
CWideString::~CWideString() { ++g_wdtors; }
const CWideString& CWideString::operator=(const CWideString& other) { PStringData = other.PStringData; return *this; }
bool CWideString::operator!=(const wchar_fable* text) const
{
    const wchar_fable* s = PStringData ? (const wchar_fable*)PStringData : kEmpty;
    if (text[0] != 0) return true;   // only compared against L"" here
    return s[0] != 0;
}
static const wchar_fable* g_plusLeft = 0;
CWideString __fastcall operator+(const wchar_fable* left, const CWideString& right)
{ g_plusLeft = left; CWideString r; r.PStringData = right.PStringData ? (void*)kParen : 0; return r; }
CWideString CCharString::ToWideString() const { CWideString r; r.PStringData = (void*)kParen; return r; }
static unsigned long g_crcSeed = 99; static const void* g_crcData = 0; static unsigned long g_crcLen = 0;
unsigned long __fastcall CCRC::Calc(unsigned long seed, const void* data, unsigned long length)
{ g_crcSeed = seed; g_crcData = data; g_crcLen = length; return length * 7; }
static int g_destroyed = 0;
void CRefVirtual::v00() {}
void CRefVirtual::Destroy() { ++g_destroyed; }
static CTextEntry g_textEntry; static CGroupEntry g_groupEntry; static const wchar_fable* g_entryText = kHello;
CWideString CTextEntry::GetText() const { CWideString r; r.PStringData = (void*)g_entryText; return r; }
static long g_lookupResult = 0; static unsigned long g_lookupCrc = 0;
long CTextDataBank::LookupValue(unsigned long crc) const { g_lookupCrc = crc; return g_lookupResult; }
unsigned long CTextDataBank::GetEntryDataType(long entry) const { return entry == 2 ? 1 : 0; }
// The /Od stubs return a named local: its own destructor drops one reference, so each getter adds two.
CIVCountedPointer<CTextEntry> CTextDataBank::GetPTextEntry(long entry, bool b) const
{ CIVCountedPointer<CTextEntry> r; r.Data = &g_textEntry; g_textEntry.m_count += 2; return r; }
CIVCountedPointer<CGroupEntry> CTextDataBank::GetPGroupEntry(long entry, bool b) const
{ CIVCountedPointer<CGroupEntry> r; r.Data = &g_groupEntry; g_groupEntry.m_count += 2; return r; }
static CTextDataBank g_bank; static CEngine_Retail g_engine;
CEngine_Retail* g_pEngine = &g_engine;
static CPlayerGui g_gui; CPlayerGui* g_pPlayerGui = &g_gui;
static int g_shown = 0; static const void* g_shownText = 0;
void CPlayerGui::AddScreenMessage(const CWideString& text) { ++g_shown; g_shownText = text.PStringData; }
static int g_randCalls = 0; static unsigned long g_randRange = 0; static unsigned long* g_randSeed = 0; static unsigned long g_seed = 5;
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed) { ++g_randCalls; g_randRange = range; g_randSeed = seed; return 0; }
unsigned long* CWorld_Retail::GetRandomSeed() { return &g_seed; }
void CGameScriptInterface_vt_dummy() {}
int main()
{
    g_engine.TextBank = &g_bank;
    static CWorld_Retail world;
    static unsigned char gsiStore[sizeof(CGameScriptInterface)];
    CGameScriptInterface& gsi = *(CGameScriptInterface*)gsiStore; gsi.World = &world;
    static char tagText[] = "TEXT_TAG"; static CCharStringData tagData = { tagText, 8 };
    CCharString tag; tag.PStringData = &tagData;
    // unknown tag: "(" + tag is shown; the CRC covers the tag's text and length
    g_lookupResult = 0;
    gsi.CGameScriptInterface::AddScreenMessage(tag, TEXT_GROUP_SELECT_FIRST);
    CHECK(g_crcSeed == 0 && g_crcData == tagText && g_crcLen == 8); CHECK(g_lookupCrc == 56);
    CHECK(g_plusLeft != 0 && g_plusLeft[0] == '(' && g_plusLeft[1] == 0);
    CHECK(g_shown == 1 && g_shownText == kParen);
    // null tag string: CRC of "" with length 0
    CCharString nulltag; nulltag.PStringData = 0;
    gsi.CGameScriptInterface::AddScreenMessage(nulltag, TEXT_GROUP_SELECT_FIRST);
    CHECK(g_crcLen == 0 && g_crcData != 0 && ((const char*)g_crcData)[0] == 0);
    // text entry: its text is shown, the temporary reference is released (bank keeps one)
    g_lookupResult = 1; g_textEntry.m_count = 1; g_shown = 0;
    gsi.CGameScriptInterface::AddScreenMessage(tag, TEXT_GROUP_SELECT_FIRST);
    CHECK(g_shown == 1 && g_shownText == kHello); CHECK(g_textEntry.m_count == 1); CHECK(g_destroyed == 0);
    // text entry holding its last reference: destroyed after use; empty text is not shown
    g_textEntry.m_count = 0; g_entryText = kEmpty; g_shown = 0;
    gsi.CGameScriptInterface::AddScreenMessage(tag, TEXT_GROUP_SELECT_FIRST);
    CHECK(g_destroyed == 1); CHECK(g_shown == 0);
    // group entry: the random pick is drawn only for a random method and more than one entry; nothing is shown
    static long ents[3]; g_groupEntry.Entries._M_start = ents; g_groupEntry.Entries._M_finish = ents + 3;
    g_lookupResult = 2; g_groupEntry.m_count = 1; g_destroyed = 0;
    gsi.CGameScriptInterface::AddScreenMessage(tag, TEXT_GROUP_SELECT_RANDOM);
    CHECK(g_randCalls == 1 && g_randRange == 3 && g_randSeed == &g_seed); CHECK(g_shown == 0); CHECK(g_groupEntry.m_count == 1);
    gsi.CGameScriptInterface::AddScreenMessage(tag, TEXT_GROUP_SELECT_FIRST);
    CHECK(g_randCalls == 1);
    g_groupEntry.Entries._M_finish = ents + 1;
    gsi.CGameScriptInterface::AddScreenMessage(tag, TEXT_GROUP_SELECT_RANDOM);
    CHECK(g_randCalls == 1); CHECK(g_destroyed == 0);
    if (fails == 0) printf("00892850_TEST PASS\n"); else printf("00892850_TEST FAIL\n");
    return fails;
}