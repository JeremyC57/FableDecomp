// Behaviour test for CTCQuestCard::GetQuestName (0x00702310): a set symbol returns the bank's text, or the widened symbol when the
// bank text is empty; without a symbol the def's text index (> 0, entry present) gives the entry's text, else the empty string;
// every temporary and the group entry reference are released.
#include <cstdio>
#include <cstring>
#include <stddef.h>
static int g_wides = 0;
struct CWideString {
    const char* PStringData;
    CWideString(const char* t) : PStringData(t) { ++g_wides; }
    CWideString(const CWideString& other) : PStringData(other.PStringData) { ++g_wides; }
    ~CWideString() { --g_wides; }
    bool operator!=(const wchar_t* text) const { return !(text[0] == 0 && PStringData[0] == 0); }
};
const CWideString g_EmptyWideString("");
struct CCharString {
    const char* PStringData;
    bool operator!=(const char* text) const { return std::strcmp(PStringData, text) != 0; }
    CWideString ToWideString() const { return CWideString(PStringData); }
};
static int g_deleted = 0;
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags) { }
    virtual void Delete() { ++g_deleted; }
    unsigned long IVRefCount;
    void DecRef() { --IVRefCount; }
    unsigned long GetRefCount() const { return IVRefCount; }
    void Release() { DecRef(); if (GetRefCount() == 0) Delete(); }
};
template <class T> struct CIVCountedPointer {
    T* Object;
    CIVCountedPointer(T* p) : Object(p) { if (p) ++p->IVRefCount; }
    CIVCountedPointer(const CIVCountedPointer& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }   // VC7.1 has no NRVO
    ~CIVCountedPointer() { if (Object) Object->Release(); }
};
struct CGroupEntry : CIVCountedPointeeBase {
    const char* m_text;
    CWideString GetText() const { return CWideString(m_text); }
};
static const char* g_bankText = ""; static CGroupEntry* g_entry = 0; static unsigned long g_askedIndex = 0;
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol) const { return CWideString(g_bankText); }
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long index, bool load) const
    {
        g_askedIndex = index;
        return CIVCountedPointer<CGroupEntry>(g_entry);
    }
};
struct CMainGameComponent { CDataBank* TextBank; };
static CDataBank g_bank; static CMainGameComponent g_game = { &g_bank };
CMainGameComponent* g_pMainGameComponent = &g_game;
struct CQuestCardDef { long QuestName; long QuestObjective; };
template <class T> struct CDefPointer {
    const T* Object;
    const T* operator->() const { return Object; }
};
struct CTCQuestCard {
    CDefPointer<CQuestCardDef> PDef;
    CCharString   QuestName;
    CCharString   ObjectiveName;
    CWideString GetQuestName() const;
    CWideString GetQuestObjective() const;
};

CWideString CTCQuestCard::GetQuestName() const
{
    if (QuestName != "") {
        CWideString text = g_pMainGameComponent->TextBank->GetTextBySymbol(QuestName);
        if (text != L"")
            return text;
        return QuestName.ToWideString();
    }
    if (PDef->QuestName > 0) {
        CIVCountedPointer<CGroupEntry> entry = g_pMainGameComponent->TextBank->GetPGroupEntry(PDef->QuestName, true);
        if (entry.Object)
            return entry.Object->GetText();
    }
    return g_EmptyWideString;
}

int main()
{
    int bad = 0;
    int base = g_wides;
    CQuestCardDef def; def.QuestName = 0; def.QuestObjective = 0;
    CTCQuestCard card; card.PDef.Object = &def; card.QuestName.PStringData = ""; card.ObjectiveName.PStringData = "";
    card.QuestName.PStringData = "SYM";
    g_bankText = "Bank text";
    { CWideString r = card.GetQuestName(); if (std::strcmp(r.PStringData, "Bank text") != 0) bad |= 1; }
    g_bankText = "";
    { CWideString r = card.GetQuestName(); if (std::strcmp(r.PStringData, "SYM") != 0) bad |= 2; }
    card.QuestName.PStringData = "";
    def.QuestName = 0;
    { CWideString r = card.GetQuestName(); if (r.PStringData[0] != 0 || g_askedIndex != 0) bad |= 4; }
    def.QuestName = 42; g_entry = 0;
    { CWideString r = card.GetQuestName(); if (r.PStringData[0] != 0 || g_askedIndex != 42) bad |= 8; }
    CGroupEntry entry; entry.IVRefCount = 0; entry.m_text = "Entry text"; g_entry = &entry;
    { CWideString r = card.GetQuestName(); if (std::strcmp(r.PStringData, "Entry text") != 0 || entry.IVRefCount != 0 || g_deleted != 1) bad |= 16; }
    if (g_wides != base) bad |= 32;
    if (bad) { std::printf("00702310_TEST FAIL\n"); return 1; }
    std::printf("00702310_TEST PASS\n");
    return 0;
}