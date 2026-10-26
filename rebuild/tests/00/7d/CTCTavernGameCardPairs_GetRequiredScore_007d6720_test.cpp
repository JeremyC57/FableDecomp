// behaviour test for 007d6720 CTCTavernGameCardPairs::GetRequiredScore: the timed game type appends the target time (one decimal) to the
// "required score" text; any other type yields an empty string; the def is released once either way
// ---- test-side string stubs (narrow text buffers; the wide type stores chars) ----
#include <cstdio>
#include <cstring>
class CWideString { public:
    char m_text[160];
    CWideString() { m_text[0] = 0; }
    CWideString(const char* s) { std::strcpy(m_text, s); }
    CWideString(const wchar_t* s) { int i = 0; for (; s[i]; ++i) m_text[i] = (char)s[i]; m_text[i] = 0; }
    CWideString(const CWideString& o) { std::strcpy(m_text, o.m_text); }
    ~CWideString() {}
    const CWideString& operator+=(const CWideString& o) { std::strcat(m_text, o.m_text); return *this; }
    bool operator!=(const wchar_t* s) const { return CWideString(s).m_text[0] != m_text[0] || std::strcmp(CWideString(s).m_text, m_text) != 0; }
};
inline CWideString operator+(const CWideString& a, const CWideString& b) { CWideString r(a); r += b; return r; }
inline CWideString operator+(const CWideString& a, const wchar_t* b) { CWideString r(a); r += CWideString(b); return r; }
class CCharString { public:
    char m_text[160];
    CCharString(const char* s, long length = -1) { (void)length; std::strcpy(m_text, s); }
    CCharString(const CCharString& o) { std::strcpy(m_text, o.m_text); }
    ~CCharString() {}
    CWideString ToWideString() const { return CWideString(m_text); }
    static CCharString FormatDouble(long digits, double value) { char b[64]; std::sprintf(b, "%.*f", (int)digits, value); return CCharString(b); }
};
inline CCharString GFIntToCharString(long v) { char b[32]; std::sprintf(b, "%ld", v); return CCharString(b); }
inline bool Same(const CWideString& w, const char* s) { return std::strcmp(w.m_text, s) == 0; }
// ---- end stubs ----
static int g_destroyed = 0;
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
};
class CTavernGameDef : public CDefVirtual, public CRefCounted { public:
    unsigned long RequiredScore;
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
    CTavernGameDef* operator->() const { return Object; }
};
static CTavernGameDef g_def;
class CTCTavernGame { public:
    virtual void v00() {}
    void GetTavernGameDef(CDefPointer* out) { ++g_def.m_count; out->Object = &g_def; }
    CWideString GetText(unsigned long id) { char b[32]; std::sprintf(b, "T%lu:", id); return CWideString(b); }
    float TargetTime;
    long  GameType;
};
struct CTCTavernGameCardPairs : CTCTavernGame {
    CWideString GetRequiredScore();
};
CWideString CTCTavernGameCardPairs::GetRequiredScore()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    switch (GameType) {
    case 1:
        return GetText(def->RequiredScore) + CCharString::FormatDouble(1, TargetTime).ToWideString();
    }
    return CCharString("", -1).ToWideString();
}
int main()
{
    g_def.RequiredScore = 7; g_def.m_count = 0;
    CTCTavernGameCardPairs game; game.TargetTime = 12.25f; game.GameType = 1;
    CWideString a = game.GetRequiredScore();
    if (!Same(a, "T7:12.2") && !Same(a, "T7:12.3")) { std::printf("007d6720_TEST FAIL\n"); return 1; }
    if (g_destroyed != 1 || g_def.m_count != 0) { std::printf("007d6720_TEST FAIL\n"); return 1; }
    game.GameType = 2;
    CWideString b = game.GetRequiredScore();
    if (!Same(b, "") || g_destroyed != 2) { std::printf("007d6720_TEST FAIL\n"); return 1; }
    std::printf("007d6720_TEST PASS\n");
    return 0;
}