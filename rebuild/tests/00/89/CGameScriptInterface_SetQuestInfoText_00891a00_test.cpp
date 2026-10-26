// Behaviour test for 0x00891a00 CGameScriptInterface::SetQuestInfoText: symbol -> CRC -> tag -> group entry text,
// stored in the quest-info Text; unknown symbols leave it alone; the entry is released.
#include <cstdio>
#include <cstring>
static unsigned long Calc(unsigned long seed, const void* data, unsigned long length)
{
    const unsigned char* p = (const unsigned char*)data; unsigned long h = seed;
    for (unsigned long i = 0; i < length; ++i) h = h * 31 + p[i];
    return h;
}
class CCharStringData { public: char* PData; unsigned long StringLength; };
static int g_strings = 0;
class CCharString {
public:
    CCharStringData* PStringData; CCharStringData block; char buf[64];
    CCharString(const char* text, long) { ++g_strings; std::strcpy(buf, text); block.PData = buf; block.StringLength = (unsigned long)std::strlen(text); PStringData = &block; }
    ~CCharString() { --g_strings; }
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->PData; }
    unsigned long GetCRC() const { return Calc(0, GetString(), GetLength()); }
};
class CWideString {
public:
    long id;
    CWideString(long i) : id(i) {}
    CWideString(const CWideString& o) : id(o.id) {}
    CWideString& operator=(const CWideString& o) { id = o.id; return *this; }
};
static int g_destroyed = 0;
namespace NGameText {
class CGroupEntry { public:
    long m_count; long textId;
    virtual ~CGroupEntry() {}
    virtual void Destroy() { ++g_destroyed; }
    CWideString GetText() const { return CWideString(textId); }
    void ReleaseTemporary() { --m_count; if (m_count == 0) Destroy(); }
};
}
template <class T> class CIVCountedPointer { public:
    T* Data;
    CIVCountedPointer(T* d) : Data(d) { if (Data) ++Data->m_count; }
    CIVCountedPointer(const CIVCountedPointer& o) : Data(o.Data) { if (Data) ++Data->m_count; }
    ~CIVCountedPointer() { if (Data) Data->ReleaseTemporary(); }
    T* operator->() const { return Data; }
};
namespace NGameText {
class CDataBank {
public:
    unsigned long knownCRC; CGroupEntry* entry;
    unsigned long GetTagFromCRC(unsigned long crc) const { return crc == knownCRC ? 7 : 0; }
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long tag, bool) const { return CIVCountedPointer<CGroupEntry>(tag == 7 ? entry : 0); }
};
}
class CEngine { public: NGameText::CDataBank* PGameTextDataBank; };
static CEngine* g_pEngine;
namespace NPlayerGui { class CDrawQuestInfo { public: CWideString Name; CWideString Text; CDrawQuestInfo() : Name(0), Text(0) {} }; }
template <class T> class CCountedPointer { public: T* Data; void* Info; };
class CPlayerGui { public:
    CCountedPointer<NPlayerGui::CDrawQuestInfo> PDrawQuestInfo;
    NPlayerGui::CDrawQuestInfo& GetQuestInfo() { return *PDrawQuestInfo.Data; }
};
static CPlayerGui* g_pPlayerGui;
class CGameScriptInterface { public: void SetQuestInfoText(const char* symbol) const; };
void CGameScriptInterface::SetQuestInfoText(const char* symbol) const
{
    NGameText::CDataBank* bank = g_pEngine->PGameTextDataBank;
    unsigned long tag = bank->GetTagFromCRC(CCharString(symbol, -1).GetCRC());
    if (tag > 0) {
        CIVCountedPointer<NGameText::CGroupEntry> entry = bank->GetPGroupEntry(tag, true);
        g_pPlayerGui->GetQuestInfo().Text = entry->GetText();
    }
}
int main()
{
    NGameText::CGroupEntry e; e.m_count = 1; e.textId = 4242;
    NGameText::CDataBank bank; bank.knownCRC = Calc(0, "TEXT_QUEST_NAME", 15); bank.entry = &e;
    CEngine engine = { &bank }; g_pEngine = &engine;
    NPlayerGui::CDrawQuestInfo info; CPlayerGui gui; gui.PDrawQuestInfo.Data = &info; g_pPlayerGui = &gui;
    CGameScriptInterface gsi;
    int fail = 0;
    gsi.SetQuestInfoText("NOT_A_SYMBOL");
    if (info.Name.id != 0 || info.Text.id != 0) fail |= 1;
    gsi.SetQuestInfoText("TEXT_QUEST_NAME");
    if (info.Text.id != 4242 || info.Name.id != 0 || e.m_count != 1 || g_destroyed || g_strings) fail |= 2;
    if (fail) { std::printf("00891a00_TEST FAIL\n"); return 1; }
    std::printf("00891a00_TEST PASS\n");
    return 0;
}