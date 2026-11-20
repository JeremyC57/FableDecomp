// Behaviour test for 0059d761: body copied from src/0059d761.cpp (NInventory::CTCInventoryTrade::FinalRefreshSubtitles), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1) : PStringData((void*)text) { CHECK(length == -1); }
    ~CCharString() { }
};
static int g_wideCtors = 0, g_wideDtors = 0;
class CWideString { public:
    void* PStringData;
    char text[64];
    CWideString() : PStringData(0) { text[0] = 0; ++g_wideCtors; }
    CWideString(const CWideString& o) : PStringData(o.PStringData) { strcpy(text, o.text); ++g_wideCtors; }
    ~CWideString() { ++g_wideDtors; }
    const CWideString& operator=(const CWideString& other) { strcpy(text, other.text); return *this; }
    const CWideString& operator+=(const CWideString& other) { strcat(text, other.text); return *this; }
};
CWideString __fastcall GetXString() { CWideString s; strcpy(s.text, " x"); return s; }
CWideString __fastcall GFIntToWideString(long value) { CWideString s; sprintf(s.text, "%ld", value); return s; }
static int g_setText = 0; static char g_lastText[64]; static const char* g_findName = 0; static long g_childType = -1;
namespace NUISystem {
struct CTextSource {
    CWideString text;
    virtual const CWideString& GetText() { return text; }
};
class CComponent { public:
    CTextSource child; CComponent* pcSubtitle;
    virtual CComponent* FindChild(CCharString name) { g_findName = (const char*)name.PStringData; return pcSubtitle; }
    virtual CTextSource* GetChildOfType(long type) { g_childType = type; return &child; }
    virtual void SetText(const CWideString& text, bool a, bool b) { ++g_setText; strcpy(g_lastText, text.text); CHECK(a && b); }
};
}
struct SSubtitleNode {
    unsigned char          _base[0x10];
    NUISystem::CComponent* Component;
    CWideString            Text;
    long                   Count;
};
static SSubtitleNode* g_found = 0; static int g_finds = 0;
struct CSubtitleMap {
    SSubtitleNode* _M_header;
    unsigned long  _M_node_count;
    long           _M_key_compare;
    SSubtitleNode* find(NUISystem::CComponent* const& key) { ++g_finds; return g_found ? g_found : _M_header; }
    SSubtitleNode* end() const { return _M_header; }
};
namespace NInventory {
struct CTCInventoryTrade {
    void FinalRefreshSubtitles(NUISystem::CComponent* component, CSubtitleMap& subtitles);
};
void CTCInventoryTrade::FinalRefreshSubtitles(NUISystem::CComponent* component, CSubtitleMap& subtitles)
{
    SSubtitleNode* found = subtitles.find(component);
    NUISystem::CComponent* owner = component;
    CWideString text;
    if (found == subtitles.end()) {
        text = owner->GetChildOfType(6)->GetText();
    } else {
        text = found->Text;
        if (found->Count > 0) {
            text += GetXString();
            text += GFIntToWideString(found->Count);
        }
    }
    owner->FindChild("PC_SUBTITLE")->SetText(text, true, true);
}
}
int main()
{
    static NUISystem::CComponent component, subtitle; component.pcSubtitle = &subtitle; strcpy(component.child.text.text, "Apple");
    static SSubtitleNode header, node; strcpy(node.Text.text, "Pear"); node.Count = 0;
    CSubtitleMap map; map._M_header = &header; map._M_node_count = 0; map._M_key_compare = 0;
    NInventory::CTCInventoryTrade trade;
    // no entry: the component's own type-6 child text
    g_found = 0; trade.FinalRefreshSubtitles(&component, map);
    CHECK(g_finds == 1 && g_childType == 6 && g_setText == 1 && strcmp(g_lastText, "Apple") == 0 && strcmp(g_findName, "PC_SUBTITLE") == 0);
    // entry without a count: its text
    g_found = &node; trade.FinalRefreshSubtitles(&component, map); CHECK(g_setText == 2 && strcmp(g_lastText, "Pear") == 0);
    // entry with a count: text + " x" + count
    node.Count = 3; trade.FinalRefreshSubtitles(&component, map); CHECK(g_setText == 3 && strcmp(g_lastText, "Pear x3") == 0);
    CHECK(g_wideDtors > 0);
    if (fails == 0) printf("0059d761_TEST PASS\n"); else printf("0059d761_TEST FAIL\n");
    return fails;
}