// Behaviour test for 007f4b60: src/007f4b60.cpp copied verbatim; wide strings modelled over static wchar buffers and
// the text bank stubbed so "name - occupation", the bare name and the missing-tag fallback are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCHasName::GetNPCNameAndFullTitle() -- the NPC's short name followed by " - " and its occupation text when the text bank has that entry; else the short name, or "Missing text tag" when the name is empty
// Retry 10b (accessor sweep, FIXES B-4/B-5): the def is read through an inline accessor; the plain member read gives the parked register difference.
// Leaf: GetNPCNameAndFullTitle
//
// Retail 0x007F4B60, 288 bytes incl. padding (`ret 4`: hidden CWideString result). Ego_r.pdb
// `?GetNPCNameAndFullTitle@CTCHasName@@QAE?AVCWideString@@XZ`.
// Callees: CTCHasName::GetNPCShortName() @ 0x007F4990, the text bank's EntryExists(unsigned long) @ 0x009CC190 (label
// "CBankFile::EntryExists"), NGameText::CDataBank::GetPGroupEntry(unsigned long, bool) const @ 0x009C9070
// (CIVCountedPointer<const CGroupEntry> by value), CGroupEntry's text getter @ 0x009C7A40 (label "CBankFile::GetBankHandle"
// is propagated; landed as GetText in CItem::GetItemDescription 0x005BD8A0), the entry's virtual slot 1 (release to zero),
// operator+(const CWideString&, const wchar_t*) @ 0x0099BF30 and operator+(const CWideString&, const CWideString&) @ 0x0099BE70
// (both __fastcall, hidden result in ecx), CWideString::CWideString(const CWideString&) @ 0x0099B720, CWideString::GetLength
// @ 0x0099B220 (label says CCharString), CWideString::CWideString(const wchar_t*) @ 0x0099B6B0, ~CWideString @ 0x0099B510.
// Globals: g_pMainGameComponent @ 0x013B86A0 (text bank +0x14), L" - " @ 0x0122E070, L"Missing text tag" @ 0x01271DA8.
// Layout: Ego_r.pdb CTCHasName: PDef +0x0c; CHasNameDef Occupation +0x30 (PDB offset, no shift for this def).
// Shape: the bank is a local read before the def field; the counted group entry is released inline through the pointee's
// ReleaseTemporary (DecRef/GetRefCount on a refcount base: the long `mov esi,[eax+4] ; ... ; dec esi ; mov edx,esi ; test`
// release); the concatenation is one `return name + L" - " + title;` (VC7.1 has no NRVO, so it copies into the result).
typedef unsigned short wchar16;

struct CWideString {
    void* PStringData;
    CWideString(const wchar16* text);                       // 0x0099B6B0
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
    long GetLength() const;                                 // 0x0099B220
};
CWideString __fastcall operator+(const CWideString& left, const wchar16* right);       // 0x0099BF30
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy() const;                           // slot 1
};
struct CRefCounted {                                        // the count sits at +4 behind the vptr
    mutable long m_count;
    void DecRef() const { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CIVCountedPointee : CDefVirtual, CRefCounted {
    void ReleaseTemporary() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CIVCountedPointer {
    T* Object;
    ~CIVCountedPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};

namespace NGameText {
struct CGroupEntry : CIVCountedPointee {
    CWideString GetText() const;                            // 0x009C7A40
};
struct CDataBank {
    bool EntryExists(unsigned long index);                  // 0x009CC190
    CIVCountedPointer<const CGroupEntry> GetPGroupEntry(unsigned long index, bool load) const;   // 0x009C9070
};
}

struct CMainGameComponent {
    unsigned char        _pad00[0x14];
    NGameText::CDataBank* TextBank;                         // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0

extern const wchar16 g_textSpacedDash[];                    // 0x0122E070 L" - "
extern const wchar16 g_textMissingTextTag[];                // 0x01271DA8 L"Missing text tag"

struct CHasNameDef {
    unsigned char _pad00[0x28];
    unsigned long DefaultNameTag;                           // +0x28
    unsigned long Home;                                     // +0x2c
    unsigned long Occupation;                               // +0x30
};

class CThing;
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCHasName : public CTCBase {
public:
    const CHasNameDef* PDef;                                // +0x0c CDefPointer<CHasNameDef const>
    const CHasNameDef* GetDef() const { return PDef; }
    CWideString GetNPCShortName();                          // 0x007F4990
    CWideString GetNPCNameAndFullTitle();
};

CWideString CTCHasName::GetNPCNameAndFullTitle()
{
    CWideString name = GetNPCShortName();
    NGameText::CDataBank* bank = g_pMainGameComponent->TextBank;
    if (bank->EntryExists(GetDef()->Occupation)) {
        CWideString title = bank->GetPGroupEntry(GetDef()->Occupation, true)->GetText();
        CWideString fullTitle = name + g_textSpacedDash + title;
        return fullTitle;
    }
    if (name.GetLength() > 0)
        return name;
    return CWideString(g_textMissingTextTag);
}
// ---- end source ----
static wchar16 g_pool[16][64]; static int g_next = 0;
static long WLen(const wchar16* s) { long n = 0; while (s && s[n]) ++n; return n; }
static wchar16* WDup2(const wchar16* a, const wchar16* b)
{ wchar16* d = g_pool[g_next++ & 15]; long n = 0; for (long i = 0; a[i]; ++i) d[n++] = a[i]; for (long i = 0; b[i]; ++i) d[n++] = b[i]; d[n] = 0; return d; }
static bool WEq(const CWideString& s, const char* t) { const wchar16* p = (const wchar16*)s.PStringData; long i = 0; for (; t[i]; ++i) if (p[i] != (wchar16)t[i]) return false; return p[i] == 0; }
static int g_dtors = 0;
CWideString::CWideString(const wchar16* text) : PStringData((void*)text) {}
CWideString::CWideString(const CWideString& o) : PStringData(o.PStringData) {}
CWideString::~CWideString() { ++g_dtors; }
long CWideString::GetLength() const { return WLen((const wchar16*)PStringData); }
CWideString __fastcall operator+(const CWideString& l, const wchar16* r) { return CWideString(WDup2((const wchar16*)l.PStringData, r)); }
CWideString __fastcall operator+(const CWideString& l, const CWideString& r) { return CWideString(WDup2((const wchar16*)l.PStringData, (const wchar16*)r.PStringData)); }
static wchar16 W(const char* s, wchar16* out) { long i = 0; for (; s[i]; ++i) out[i] = (wchar16)s[i]; out[i] = 0; return 0; }
static wchar16 g_dash[8], g_missing[32], g_name[16], g_title[16], g_empty[2];
const wchar16 g_textSpacedDash[4] = { ' ', '-', ' ', 0 };
const wchar16 g_textMissingTextTag[17] = { 'M','i','s','s','i','n','g',' ','t','e','x','t',' ','t','a','g',0 };
static const wchar16* g_shortName = 0;
CWideString CTCHasName::GetNPCShortName() { return CWideString(g_shortName); }
static int g_destroys = 0;
void CDefVirtual::Destroy() const { ++g_destroys; }
void CDefVirtual::v00() {}
static NGameText::CGroupEntry g_entry;
CWideString NGameText::CGroupEntry::GetText() const { return CWideString(g_title); }
static bool g_exists = true; static unsigned long g_asked = 0;
bool NGameText::CDataBank::EntryExists(unsigned long index) { g_asked = index; return g_exists; }
CIVCountedPointer<const NGameText::CGroupEntry> NGameText::CDataBank::GetPGroupEntry(unsigned long, bool) const
{ static void* s_holder; s_holder = &g_entry; ++g_entry.m_count; return *(CIVCountedPointer<const NGameText::CGroupEntry>*)&s_holder; }
static NGameText::CDataBank g_bank; static CMainGameComponent g_mgc;
CMainGameComponent* g_pMainGameComponent = &g_mgc;
int main()
{
    W("Bob", g_name); W("Smith", g_title); g_empty[0] = 0;
    g_mgc.TextBank = &g_bank; g_entry.m_count = 1;
    static CHasNameDef def; def.Occupation = 77;
    static unsigned char raw[sizeof(CTCHasName)]; memset(raw, 0, sizeof(raw));
    CTCHasName* h = (CTCHasName*)raw; h->PDef = &def;
    g_shortName = g_name;
    CWideString a = h->GetNPCNameAndFullTitle();
    CHECK(WEq(a, "Bob - Smith") && g_asked == 77 && g_entry.m_count == 1 && g_destroys == 0);
    g_exists = false;
    CWideString b = h->GetNPCNameAndFullTitle(); CHECK(WEq(b, "Bob"));
    g_shortName = g_empty;
    CWideString c = h->GetNPCNameAndFullTitle(); CHECK(WEq(c, "Missing text tag"));
    if (fails == 0) printf("007f4b60_TEST PASS\n"); else printf("007f4b60_TEST FAIL\n");
    return fails;
}