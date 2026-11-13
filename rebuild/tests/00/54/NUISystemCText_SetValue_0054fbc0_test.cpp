// Behaviour test for NUISystem::CText::SetValue 0x0054FBC0: the same decision logic over a model bank.
#include <cstdio>
#include <string>
#include <vector>
#include <map>
struct Bank {
    std::map<unsigned long, std::wstring> texts;
    std::map<unsigned long, std::vector<long> > groups;
    unsigned long GetEntryDataType(unsigned long id) const { return groups.count(id) ? 1 : 0; }
};
static Bank* g_game = 0;
static Bank* g_front = 0;
static bool g_formatCalled = false;
static std::wstring Format(const std::wstring& s) { g_formatCalled = true; return s; }
static long FitInWindow(const std::wstring& s) { long n = 1; for (size_t i = 0; i < s.size(); ++i) if (s[i] == L'\n') ++n; return n; }
struct Text { std::wstring Value; float SizeX, SizeY;
    void SetValue(unsigned long textId, const std::wstring& value, bool spaceSeparated) {
        Bank* bank = textId > 0 ? (g_game ? g_game : g_front) : 0;
        if (!bank) Value = value;
        else if (!bank->GetEntryDataType(textId)) Value = bank->texts[textId];
        else {
            std::vector<long> ids = bank->groups[textId];
            Value = L"";
            for (unsigned long i = 0; i < ids.size(); ++i) {
                if (Value.size()) Value += spaceSeparated ? L" " : L"\n";
                Value += bank->texts[(unsigned long)ids[i]];
            }
        }
        if (g_game) Value = Format(Value);
        SizeY = (float)FitInWindow(Value);
        SizeX = (float)Value.size() * 8.0f;
    }
};
int main()
{
    Bank b; b.texts[1] = L"Hello"; b.texts[2] = L"World"; b.groups[3].push_back(1); b.groups[3].push_back(2);
    Text t; bool ok = true;
    t.SetValue(0, L"raw", true); ok = ok && t.Value == L"raw" && !g_formatCalled && t.SizeY == 1.0f;
    t.SetValue(1, L"raw", true); ok = ok && t.Value == L"raw";            // no bank at all
    g_front = &b;
    t.SetValue(1, L"raw", true); ok = ok && t.Value == L"Hello" && !g_formatCalled;
    t.SetValue(3, L"raw", false); ok = ok && t.Value == L"Hello\nWorld" && t.SizeY == 2.0f;
    g_game = &b;
    t.SetValue(3, L"raw", true); ok = ok && t.Value == L"Hello World" && g_formatCalled && t.SizeX == 88.0f;
    if (!ok) { std::printf("0054fbc0_TEST FAIL\n"); return 1; }
    std::printf("0054fbc0_TEST PASS\n");
    return 0;
}