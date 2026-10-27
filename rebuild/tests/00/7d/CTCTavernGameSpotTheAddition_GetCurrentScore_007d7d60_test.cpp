// behaviour test for 0x007D7D60 CTCTavernGameSpotTheAddition::GetCurrentScore() (model; the string layer and the def lookup are stubs)
#include <cstdio>
#include <cstring>
#include <string>
static std::string g_log;
static int g_released = 0;
class CWideString { public: std::string s; };
class CCharString { public: std::string s; CWideString ToWideString() const { CWideString w; w.s = s; return w; }
    static CCharString FormatDouble(long digits, double value) { char b[64]; std::sprintf(b, "%.*f", (int)digits, value); CCharString c; c.s = b; return c; } };
CWideString operator+(const CWideString& a, const CWideString& b) { CWideString w; w.s = a.s + b.s; return w; }
class CTavernGameDef { public: long m_count; unsigned long CurrentScore, RequiredScore; void ReleaseTemporary() { if (--m_count == 0) ++g_released; } };
struct CDefPointer { CTavernGameDef* Object; CDefPointer() : Object(0) {} ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameDef* operator->() const { return Object; } };
static CTavernGameDef g_def;
class CGame { public:
    float CurrentTime;
    void GetTavernGameDef(CDefPointer* out) { ++g_def.m_count; out->Object = &g_def; }
    CWideString GetText(unsigned long id) { CWideString w; w.s = id == 7 ? "Score: " : "Need: "; return w; }
    CWideString GetCurrentScore()
    {
        CDefPointer def;
        GetTavernGameDef(&def);
        return GetText(def->CurrentScore) + CCharString::FormatDouble(1, CurrentTime).ToWideString();
    }
};
int main()
{
    g_def.m_count = 0; g_def.CurrentScore = 7; g_def.RequiredScore = 9;
    CGame g; g.CurrentTime = 12.34f;
    CWideString r = g.GetCurrentScore();
    int ok = r.s == std::string("Score: 12.3") && g_released == 1 && g_def.m_count == 0;
    if (!ok) { std::printf("007d7d60_TEST FAIL %s\n", r.s.c_str()); return 1; }
    std::printf("007d7d60_TEST PASS\n");
    return 0;
}