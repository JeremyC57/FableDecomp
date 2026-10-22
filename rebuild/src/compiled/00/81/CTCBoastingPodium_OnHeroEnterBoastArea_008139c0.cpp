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
struct CFontBank;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    unsigned long GetCRC() const;                          // 0x00404310
};
struct CWideString {
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
struct CIVCountedPointeeBase : CIVCountedVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
template <class T> struct CIVCountedPointer {
    T* Object;
    ~CIVCountedPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
    T* operator->() const { return Object; }
};
namespace NGameText {
struct CGroupEntry : CIVCountedPointeeBase {
    CWideString GetText() const;                           // 0x009C7A40
};
struct CDataBank {
    unsigned long LookupValue(unsigned long key) const;    // 0x009CC410
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long index, bool load) const;   // 0x009C9070
};
}
struct CMainGameComponent {
    unsigned char         _pad00[0x14];
    NGameText::CDataBank* TextBank;                        // +0x14
    unsigned char         _pad18[0x1614c - 0x18];
    CFontBank*            Font;                            // +0x1614C
};
extern CMainGameComponent* g_pMainGameComponent;           // 0x013B86A0

struct CSubtitleRenderer {
    void SetText(CFontBank* font, const CWideString& text, const CWideString& button1, const CWideString& button2,
                 const CWideString& button3, unsigned long colour, float time, bool a, bool b);   // 0x00492F69
    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1
};
struct CPlayerGui {
    unsigned char     _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                    // +0x18
};
extern CPlayerGui* g_pPlayerGui;                           // 0x013B8790

struct CQuestManager {
    struct CBoast {
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

struct CThingCreatureBase {
    unsigned char _pad00[0x10];
    unsigned char ThingClass;                              // +0x10 (2: the hero)
};

struct CTCBoastingPodium {
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