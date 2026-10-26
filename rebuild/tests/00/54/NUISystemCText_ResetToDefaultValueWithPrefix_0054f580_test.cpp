#include <cstdio>
#include <cstring>
class CWideString { public:
    char text[32];
    CWideString(const char* t) { std::strcpy(text, t); }
    CWideString(const CWideString& o) { std::strcpy(text, o.text); }
};
CWideString operator+(const CWideString& l, const CWideString& r) { CWideString s(l); std::strcat(s.text, r.text); return s; }
static char g_seq[8] = "";
struct CText {
    CWideString Text; bool A, B;
    CText() : Text("old"), A(true), B(false) { }
    virtual ~CText() { }
    virtual const CWideString& GetText() const { std::strcat(g_seq, "T"); return Text; }
    virtual void SetText(const CWideString& t, bool a, bool b) { std::strcat(g_seq, "S"); Text = t; A = a; B = b; }
    void GetDataFromDef() { std::strcat(g_seq, "G"); Text = CWideString("def"); }
    void ResetToDefaultValueWithPrefix(const CWideString& prefix) { GetDataFromDef(); SetText(prefix + GetText(), false, true); }
};
int main()
{
    CText t;
    t.ResetToDefaultValueWithPrefix(CWideString("x "));
    if (std::strcmp(t.Text.text, "x def") || t.A || !t.B || std::strcmp(g_seq, "GTS")) { std::printf("0054f580_TEST FAIL\n"); return 1; }
    std::printf("0054f580_TEST PASS\n");
    return 0;
}