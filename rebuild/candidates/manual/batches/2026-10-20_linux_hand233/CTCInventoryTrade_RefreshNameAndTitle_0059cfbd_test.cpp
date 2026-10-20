// Behaviour test for CTCInventoryTrade::RefreshNameAndTitle (0x0059CFBD).
#include <cstdio>
#include <cstring>

struct CWideString {
    char s[64];
    CWideString() { s[0] = 0; }
    CWideString(const char* t) { std::strcpy(s, t); }
    CWideString& operator+=(const CWideString& o) { std::strcat(s, o.s); return *this; }
};
CWideString operator+(const CWideString& a, const CWideString& b) { CWideString r(a.s); r += b; return r; }
static CWideString GetXString() { return CWideString(" x "); }
static CWideString IntToWideString(long v) { char b[16]; std::sprintf(b, "%ld", v); return CWideString(b); }
static CWideString GetDescription(long defIndex) { return CWideString(defIndex == 7 ? "Apple" : "Key"); }

struct CComponent {
    const char* Name;
    CComponent* Children[2];
    CWideString Text; int Sets;
    CComponent* FindChildRecursive(const char* n) { for (int i = 0; i < 2; ++i) if (Children[i] && std::strcmp(Children[i]->Name, n) == 0) return Children[i]; return 0; }
    CComponent* FindChildOfAnyDepth(const char* n) { return FindChildRecursive(n); }
    void SetText(const CWideString& t, bool, bool) { Text = t; ++Sets; }
};
struct CTCInventoryTrade {
    bool IsSpecialCase(long defIndex) { return defIndex == 9; }
    void RefreshNameAndTitle(CComponent* entry, long defIndex, long count)
    {
        CComponent* text = entry->FindChildRecursive("MENU_ENTRY_TEXT");
        CComponent* subtitle = entry->FindChildOfAnyDepth("UI_TEXT_TRADE_NAME_TEMPLATE_NEW");
        CWideString name;
        if (text || subtitle) {
            name = GetDescription(defIndex);
            if (count > 1 && !IsSpecialCase(defIndex))
                name += GetXString() + IntToWideString(count);
            if (text)
                text->SetText(name, true, true);
            if (subtitle)
                subtitle->SetText(name, true, true);
        }
    }
};

int main()
{
    CComponent text = { "MENU_ENTRY_TEXT", { 0, 0 }, CWideString(), 0 };
    CComponent sub = { "UI_TEXT_TRADE_NAME_TEMPLATE_NEW", { 0, 0 }, CWideString(), 0 };
    CComponent entry = { "ENTRY", { &text, &sub }, CWideString(), 0 };
    CTCInventoryTrade t;
    t.RefreshNameAndTitle(&entry, 7, 3);
    bool ok = std::strcmp(text.Text.s, "Apple x 3") == 0 && std::strcmp(sub.Text.s, "Apple x 3") == 0;
    t.RefreshNameAndTitle(&entry, 9, 3);                          // special: no multiplier
    ok = ok && std::strcmp(text.Text.s, "Key") == 0 && sub.Sets == 2;
    t.RefreshNameAndTitle(&entry, 7, 1);                          // single item
    ok = ok && std::strcmp(text.Text.s, "Apple") == 0;
    CComponent bare = { "BARE", { 0, 0 }, CWideString(), 0 };
    t.RefreshNameAndTitle(&bare, 7, 3);                           // no children: nothing set
    ok = ok && text.Sets == 3;
    if (!ok) { std::printf("0059cfbd_TEST FAIL\n"); return 1; }
    std::printf("0059cfbd_TEST PASS\n");
    return 0;
}
