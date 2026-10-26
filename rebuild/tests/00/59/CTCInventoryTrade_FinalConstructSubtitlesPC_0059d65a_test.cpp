// Behaviour test for CTCInventoryTrade::FinalConstructSubtitlesPC (0x0059D65A).
#include <cstdio>
#include <cstring>

class CWideString { public:
    char s[64];
    CWideString() { s[0] = 0; }
    CWideString(const char* t) { std::strcpy(s, t); }
    CWideString& operator+=(const CWideString& o) { std::strcat(s, o.s); return *this; }
};
static CWideString GetXString() { return CWideString(" x "); }
static CWideString IntToWideString(long v) { char b[16]; std::sprintf(b, "%ld", v); return CWideString(b); }

struct CComponent {
    char Def[32];
    CWideString Text;
    CWideString OwnText;                          // what GetChildOfType(6)->GetText() returns
    CComponent* Added; CComponent* Registered;
    const CWideString& GetOwnText() { return OwnText; }
    void SetText(const CWideString& t, bool, bool) { Text = t; }
    void AddChild(CComponent* c) { Added = c; }
    void RegisterSubtitle(CComponent* c) { Registered = c; }
};
static CComponent g_created;
static CComponent* CreateComponent(const char* def) { std::strcpy(g_created.Def, def); return &g_created; }
struct SSubtitleNode { CComponent* Component; CWideString Text; long Count; };
struct CSubtitleMap {
    SSubtitleNode Nodes[2]; int Size; SSubtitleNode End;
    SSubtitleNode* find(CComponent* key) { for (int i = 0; i < Size; ++i) if (Nodes[i].Component == key) return &Nodes[i]; return &End; }
    SSubtitleNode* end() { return &End; }
};
static void Construct(CComponent* component, CSubtitleMap& subtitles, CComponent* list)
{
    SSubtitleNode* found = subtitles.find(component);
    CComponent* owner = component;
    CWideString text;
    if (found == subtitles.end()) {
        text = owner->GetOwnText();
    } else {
        text = found->Text;
        if (found->Count > 0) {
            text += GetXString();
            text += IntToWideString(found->Count);
        }
    }
    CComponent* subtitle = CreateComponent("PC_SUBTITLE");
    subtitle->SetText(text, true, true);
    owner->AddChild(subtitle);
    if (1) list->RegisterSubtitle(subtitle);
}

int main()
{
    CComponent a; std::memset(&a, 0, sizeof(a)); std::strcpy(a.OwnText.s, "Own");
    CComponent list; std::memset(&list, 0, sizeof(list));
    CSubtitleMap map; std::memset(&map, 0, sizeof(map));
    Construct(&a, map, &list);                            // not in the map: the own text
    bool ok = std::strcmp(g_created.Def, "PC_SUBTITLE") == 0 && std::strcmp(g_created.Text.s, "Own") == 0 && a.Added == &g_created;
    map.Nodes[0].Component = &a; std::strcpy(map.Nodes[0].Text.s, "Sword"); map.Nodes[0].Count = 2; map.Size = 1;
    Construct(&a, map, &list);
    ok = ok && std::strcmp(g_created.Text.s, "Sword x 2") == 0;
    map.Nodes[0].Count = 0;
    Construct(&a, map, &list);
    ok = ok && std::strcmp(g_created.Text.s, "Sword") == 0;
    ok = ok && (list.Registered == (1 ? &g_created : 0));
    if (!ok) { std::printf("0059d65a_TEST FAIL\n"); return 1; }
    std::printf("0059d65a_TEST PASS\n");
    return 0;
}