// Behaviour test for 008139c0: src/008139c0.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CTCBoastingPodium::OnHeroEnterBoastArea(long, bool, CThingCreatureBase&)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCBoastingPodium::OnHeroEnterBoastArea(long boastID, bool canAfford, CThingCreatureBase& hero) -- when the hero steps onto a boast's spot, show the boast text with its "gold: <cost>" button, or the "not enough money" subtitle when the hero cannot afford it
// Leaf: ?OnHeroEnterBoastArea@CTCBoastingPodium@@QAEXJ_NAAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008139C0, 430 bytes, `ret 0Ch`; `this` is not read.
// Callees: CQuestManager::GetBoast(long, CCharString) 0x004AF7A0 (g_pQuestManager 0x013B89FC; "" 0x0122D70E built in the
//   by-value argument slot), CCharString(const char*, long) 0x0099EBF0 ("TEXT_GUI_GOLD" 0x01255A38, "You do not have enough
//   money to make this boast!" 0x01272420) / ~CCharString 0x0099EAE0, CCharString::GetCRC 0x00404310, NGameText::CDataBank::
//   LookupValue 0x009CC410 / GetPGroupEntry 0x009C9070 (g_pMainGameComponent 0x013B86A0 +0x14), CGroupEntry::GetText
//   0x009C7A40 (manifest "CBankFile::GetBankHandle"), IntToWideString 0x00405F00, operator+(const wchar_t*, const
//   CWideString&) 0x0099BFF0 (L": " 0x01236A50), CWideString::operator+= 0x0099B8D0, CWideString(const wchar_t*) 0x0099B6B0
//   (L"" 0x0122D70C), ~CWideString 0x0099B510, CSubtitleRenderer::SetText 0x00492F69 (four wide texts) / 0x00492ED1
//   (narrow text) on g_pPlayerGui 0x013B8790 +0x18 (font g_pMainGameComponent +0x1614C).
// Layout: CQuestManager::CBoast Cost +0x04, Text +0x10 (Ego_r.pdb).
// Shape: every temporary lives in a dead argument slot. The quest manager and the first text-bank read are named locals
//   (loaded before the argument pushes). goldText sits in a nested `if (entry)` block, which frees the canAfford slot for
//   the "TEXT_GUI_GOLD" temporary, so both branches build their CCharString in the same word (the lea is hoisted above the
//   branch). The second bank read is a named local in that block.
class CFontBank;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    unsigned long GetCRC() const;                          // 0x00404310
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);               // 0x0099B6B0
    CWideString(const CWideString& other);                 // 0x0099B720
    ~CWideString();                                        // 0x0099B510
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
};
CWideString __fastcall operator+(const unsigned short* left, const CWideString& right);   // 0x0099BFF0
CWideString __fastcall IntToWideString(long value);       // 0x00405F00

struct CRefCounted {
    unsigned long m_count;
    void DecRef() { --m_count; }
    unsigned long GetRefCount() const { return m_count; }
};
struct CIVCountedVirtual {
    virtual void Destroy(int flags);
    virtual void Delete();                                 // slot 1
};
class CIVCountedPointeeBase : public CIVCountedVirtual, public CRefCounted { public:
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
template <class T> class CIVCountedPointer { public:
    T* Object;
    ~CIVCountedPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
    T* operator->() const { return Object; }
};
namespace NGameText {
class CGroupEntry : public CIVCountedPointeeBase { public:
    CWideString GetText() const;                           // 0x009C7A40
};
class CDataBank { public:
    unsigned long LookupValue(unsigned long key) const;    // 0x009CC410
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long index, bool load) const;   // 0x009C9070
};
}
class CMainGameComponent { public:
    unsigned char         _pad00[0x14];
    NGameText::CDataBank* TextBank;                        // +0x14
    unsigned char         _pad18[0x1614c - 0x18];
    CFontBank*            Font;                            // +0x1614C
};
extern CMainGameComponent* g_pMainGameComponent;           // 0x013B86A0

class CSubtitleRenderer { public:
    void SetText(CFontBank* font, const CWideString& text, const CWideString& button1, const CWideString& button2,
                 const CWideString& button3, unsigned long colour, float time, bool a, bool b);   // 0x00492F69
    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1
};
class CPlayerGui { public:
    unsigned char     _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                    // +0x18
};
extern CPlayerGui* g_pPlayerGui;                           // 0x013B8790

class CQuestManager { public:
    class CBoast { public:
        long        ID;                                    // +0x00
        long        Cost;                                  // +0x04
        long        Reward;                                // +0x08
        CCharString ScriptName;                            // +0x0c
        CWideString Text;                                  // +0x10
        CWideString Title;                                 // +0x14
    };
    CBoast* GetBoast(long id, CCharString scriptName);     // 0x004AF7A0
};
extern CQuestManager* g_pQuestManager;                     // 0x013B89FC

class CThingCreatureBase { public:
    unsigned char _pad00[0x10];
    unsigned char ThingClass;                              // +0x10 (2: the hero)
};

class CTCBoastingPodium { public:
    void OnHeroEnterBoastArea(long boastID, bool canAfford, CThingCreatureBase& hero);
};

void CTCBoastingPodium::OnHeroEnterBoastArea(long boastID, bool canAfford, CThingCreatureBase& hero)
{
    CQuestManager* questManager = g_pQuestManager;
    CQuestManager::CBoast* boast = questManager->GetBoast(boastID, "");
    if (!boast || hero.ThingClass != 2)
        return;
    if (canAfford) {
        NGameText::CDataBank* textBank = g_pMainGameComponent->TextBank;
        unsigned long entry = textBank->LookupValue(CCharString("TEXT_GUI_GOLD").GetCRC());
        if (entry) {
            NGameText::CDataBank* bank = g_pMainGameComponent->TextBank;
            CWideString goldText = bank->GetPGroupEntry(entry, true)->GetText();
            goldText += L": " + IntToWideString(boast->Cost);
            g_pPlayerGui->SubtitleRenderer.SetText(g_pMainGameComponent->Font, boast->Text, goldText, L"", L"", 0, 2.5f, false, true);
        }
    } else
        g_pPlayerGui->SubtitleRenderer.SetText(g_pMainGameComponent->Font, "You do not have enough money to make this boast!", 0, 2.5f, false, true);
}

// ---- stubs ----
// String buffers are reference counted ([long refs][chars]): VC7.1 has no NRVO, so by-value returns copy through the
// counted copy constructors and the local's destructor drops its own reference.
static int g_live = 0;
static char g_log[512]; static int g_step = 0;
static void Log(const char* s) { char b[64]; _snprintf(b, sizeof b, "%d:%s;", ++g_step, s); strcat(g_log, b); }
static void* NewBuf(size_t bytes) { char* b = (char*)malloc(sizeof(long) + bytes); *(long*)b = 1; ++g_live; return b; }
static void Drop(void* p) { if (p && --*(long*)p == 0) { free(p); --g_live; } }
static void Share(void* p) { if (p) ++*(long*)p; }
static const char* NStr(const CCharString& s) { return s.PStringData ? (const char*)s.PStringData + sizeof(long) : ""; }
static const unsigned short* WStr(const CWideString& s) { static const unsigned short z = 0; return s.PStringData ? (const unsigned short*)((char*)s.PStringData + sizeof(long)) : &z; }
static size_t WLen(const unsigned short* w) { size_t n = 0; while (w[n]) ++n; return n; }
static void Narrow(char* out, size_t cap, const CWideString& s) { const unsigned short* w = WStr(s); size_t i = 0; for (; w[i] && i + 1 < cap; ++i) out[i] = (char)w[i]; out[i] = 0; }
static void SetWide(CWideString& s, const unsigned short* w, size_t n) { s.PStringData = NewBuf((n + 1) * 2); unsigned short* d = (unsigned short*)((char*)s.PStringData + sizeof(long)); memcpy(d, w, n * 2); d[n] = 0; }

CCharString::CCharString(const char* text, long length) { size_t n = length < 0 ? strlen(text) : (size_t)length; PStringData = NewBuf(n + 1); memcpy((char*)PStringData + sizeof(long), text, n); ((char*)PStringData)[sizeof(long) + n] = 0; }
CCharString::CCharString(const CCharString& other) { PStringData = other.PStringData; Share(PStringData); }
CCharString::~CCharString() { Drop(PStringData); }
static char g_crcOf[64];
unsigned long CCharString::GetCRC() const { Log("GetCRC"); strcpy(g_crcOf, NStr(*this)); return 0xC0FFEE; }

CWideString::CWideString(const unsigned short* text) { SetWide(*this, text, WLen(text)); }
CWideString::CWideString(const CWideString& other) { PStringData = other.PStringData; Share(PStringData); }
CWideString::~CWideString() { Drop(PStringData); }
const CWideString& CWideString::operator+=(const CWideString& other)
{
    Log("Append");
    const unsigned short* a = WStr(*this); const unsigned short* b = WStr(other);
    size_t na = WLen(a), nb = WLen(b);
    unsigned short* t = (unsigned short*)malloc((na + nb + 1) * 2); memcpy(t, a, na * 2); memcpy(t + na, b, nb * 2);
    void* old = PStringData; SetWide(*this, t, na + nb); free(t); Drop(old);
    return *this;
}
CWideString __fastcall operator+(const unsigned short* left, const CWideString& right)
{
    Log("Concat");
    CWideString r(left);
    r += right;
    return r;
}
CWideString __fastcall IntToWideString(long value)
{
    Log("IntToWide");
    char b[32]; _snprintf(b, sizeof b, "%ld", value); unsigned short w[32]; size_t i = 0; for (; b[i]; ++i) w[i] = b[i]; w[i] = 0;
    CWideString r(w);
    return r;
}

static int g_deleted = 0;
void CIVCountedVirtual::Destroy(int) { }
void CIVCountedVirtual::Delete() { ++g_deleted; }
static NGameText::CGroupEntry g_entry;
static unsigned long g_lookupKey = 0, g_lookupResult = 0, g_groupIndex = 0; static bool g_groupLoad = false;
static NGameText::CDataBank g_bank;
unsigned long NGameText::CDataBank::LookupValue(unsigned long key) const { Log("Lookup"); g_lookupKey = key; return g_lookupResult; }
CIVCountedPointer<NGameText::CGroupEntry> NGameText::CDataBank::GetPGroupEntry(unsigned long index, bool load) const
{
    Log("GetGroupEntry"); g_groupIndex = index; g_groupLoad = load;
    CIVCountedPointer<NGameText::CGroupEntry> r;
    r.Object = &g_entry;
    g_entry.m_count += 2;   // the caller's temporary and the local the bitwise copy came from each release one
    return r;
}
static unsigned short g_goldW[] = { 'G', 'o', 'l', 'd', 0 };
CWideString NGameText::CGroupEntry::GetText() const { Log("GetText"); CWideString r(g_goldW); return r; }

static CMainGameComponent* g_mgc;
CMainGameComponent* g_pMainGameComponent;
static CPlayerGui g_gui;
CPlayerGui* g_pPlayerGui = &g_gui;
static int g_fontTag;
static char g_t0[64], g_t1[64], g_t2[64], g_t3[64]; static unsigned long g_colour; static float g_time; static bool g_a, g_b;
static void* g_renderer; static CFontBank* g_font;
void CSubtitleRenderer::SetText(CFontBank* font, const CWideString& text, const CWideString& button1, const CWideString& button2,
                                const CWideString& button3, unsigned long colour, float time, bool a, bool b)
{
    Log("SetText4"); g_renderer = this; g_font = font; Narrow(g_t0, sizeof g_t0, text); Narrow(g_t1, sizeof g_t1, button1);
    Narrow(g_t2, sizeof g_t2, button2); Narrow(g_t3, sizeof g_t3, button3); g_colour = colour; g_time = time; g_a = a; g_b = b;
}
void CSubtitleRenderer::SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b)
{
    Log("SetTextN"); g_renderer = this; g_font = font; strcpy(g_t0, NStr(text)); g_colour = colour; g_time = time; g_a = a; g_b = b;
}

static CQuestManager g_qm;
CQuestManager* g_pQuestManager = &g_qm;
static CQuestManager::CBoast* g_boastResult = 0; static long g_boastId = 0; static char g_boastScript[32];
CQuestManager::CBoast* CQuestManager::GetBoast(long id, CCharString scriptName)
{
    Log("GetBoast"); g_boastId = id; strcpy(g_boastScript, NStr(scriptName)); return g_boastResult;
}

static void Reset() { g_log[0] = 0; g_step = 0; g_boastId = 0; g_boastScript[0] = 'x'; g_boastScript[1] = 0; g_crcOf[0] = 0; g_lookupKey = 0;
    g_groupIndex = 0; g_groupLoad = false; g_t0[0] = g_t1[0] = g_t2[0] = g_t3[0] = 0; g_colour = 99; g_time = 0; g_a = true; g_b = false;
    g_renderer = 0; g_font = 0; g_deleted = 0; g_entry.m_count = 5; }

int main()
{
    g_mgc = (CMainGameComponent*)calloc(1, sizeof(CMainGameComponent));
    g_mgc->TextBank = &g_bank; g_mgc->Font = (CFontBank*)&g_fontTag;
    g_pMainGameComponent = g_mgc;
    static unsigned short textW[] = { 'B', 'o', 'a', 's', 't', 0 };
    CQuestManager::CBoast* boast = (CQuestManager::CBoast*)calloc(1, sizeof(CQuestManager::CBoast));
    boast->Cost = 250; SetWide(boast->Text, textW, 5);
    CThingCreatureBase hero; memset(&hero, 0, sizeof hero); hero.ThingClass = 2;
    CTCBoastingPodium podium;
    int live0 = g_live;
    // 1: affordable boast: "Boast" with the gold button "Gold: 250" and two empty buttons
    {
        Reset(); g_boastResult = boast; g_lookupResult = 33;
        podium.OnHeroEnterBoastArea(7, true, hero);
        CHECK(strcmp(g_log, "1:GetBoast;2:GetCRC;3:Lookup;4:GetGroupEntry;5:GetText;6:IntToWide;7:Concat;8:Append;9:Append;10:SetText4;") == 0);
        CHECK(g_boastId == 7 && strcmp(g_boastScript, "") == 0);
        CHECK(strcmp(g_crcOf, "TEXT_GUI_GOLD") == 0 && g_lookupKey == 0xC0FFEE && g_groupIndex == 33 && g_groupLoad);
        CHECK(strcmp(g_t0, "Boast") == 0 && strcmp(g_t1, "Gold: 250") == 0 && g_t2[0] == 0 && g_t3[0] == 0);
        CHECK(g_font == (CFontBank*)&g_fontTag && g_renderer == &g_gui.SubtitleRenderer);
        CHECK(g_colour == 0 && g_time == 2.5f && !g_a && g_b);
        CHECK(g_entry.m_count == 5 && g_deleted == 0 && g_live == live0);
    }
    // 2: cannot afford: the plain "not enough money" subtitle, no text lookups
    {
        Reset(); g_boastResult = boast;
        podium.OnHeroEnterBoastArea(3, false, hero);
        CHECK(strcmp(g_log, "1:GetBoast;2:SetTextN;") == 0);
        CHECK(strcmp(g_t0, "You do not have enough money to make this boast!") == 0 && g_colour == 0 && g_time == 2.5f && !g_a && g_b);
        CHECK(g_font == (CFontBank*)&g_fontTag && g_live == live0);
    }
    // 3: no gold text entry: nothing shown
    {
        Reset(); g_boastResult = boast; g_lookupResult = 0;
        podium.OnHeroEnterBoastArea(4, true, hero);
        CHECK(strcmp(g_log, "1:GetBoast;2:GetCRC;3:Lookup;") == 0 && g_live == live0);
    }
    // 4: unknown boast, or not the hero: nothing at all
    {
        Reset(); g_boastResult = 0;
        podium.OnHeroEnterBoastArea(5, true, hero);
        CHECK(strcmp(g_log, "1:GetBoast;") == 0);
        Reset(); g_boastResult = boast; hero.ThingClass = 1;
        podium.OnHeroEnterBoastArea(5, false, hero);
        CHECK(strcmp(g_log, "1:GetBoast;") == 0 && g_live == live0);
        hero.ThingClass = 2;
    }
    // 5: the last reference to the group entry is released by the temporary
    {
        Reset(); g_boastResult = boast; g_lookupResult = 9; g_entry.m_count = 0;
        podium.OnHeroEnterBoastArea(6, true, hero);
        CHECK(g_entry.m_count == 0 && g_deleted == 1);
    }
    if (fails == 0) printf("008139c0_TEST PASS\n");
    return fails;
}