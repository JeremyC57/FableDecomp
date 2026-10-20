// behaviour test for 007d6560 CTCTavernGameCardPairs::GetBestScore: in the timed game type the "best score" text gets the hero's best time
// (one decimal) appended when one is recorded; another type gives an empty string; the def is released either way
// ---- test-side string stubs (narrow text buffers; the wide type stores chars) ----
#include <cstdio>
#include <cstring>
struct CWideString {
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
struct CCharString {
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
struct CTavernGameDef : CDefVirtual, CRefCounted {
    unsigned long BestScore;
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
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameDef* operator->() const { return Object; }
};
struct CTCHeroStats {
    float BestTimePairs;
    float BestTimeSorting;
};
static CTavernGameDef g_def;
static CTCHeroStats g_stats;
struct CTCTavernGame {
    virtual void v00() {}
    void GetTavernGameDef(CDefPointer* out) { ++g_def.m_count; out->Object = &g_def; }
    void GetHeroStats(CTCHeroStats** stats) { *stats = &g_stats; }
    CWideString GetText(unsigned long id) { char b[32]; std::sprintf(b, "B%lu:", id); return CWideString(b); }
    long          GameType;
};
struct CTCTavernGameCardPairs : CTCTavernGame {
    CWideString GetBestScore();
};
CWideString CTCTavernGameCardPairs::GetBestScore()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    CTCHeroStats* stats = 0;
    GetHeroStats(&stats);
    CWideString text = GetText(def->BestScore);
    switch (GameType) {
    case 1:
        if (stats->BestTimePairs != 0.0f)
            text += CCharString::FormatDouble(1, stats->BestTimePairs).ToWideString();
        return text;
    }
    return CCharString("", -1).ToWideString();
}
int main()
{
    g_def.BestScore = 3; g_def.m_count = 0;
    CTCTavernGameCardPairs game; game.GameType = 1;
    g_stats.BestTimePairs = 0.0f;
    if (!Same(game.GetBestScore(), "B3:")) { std::printf("007d6560_TEST FAIL\n"); return 1; }
    g_stats.BestTimePairs = 41.5f;
    if (!Same(game.GetBestScore(), "B3:41.5")) { std::printf("007d6560_TEST FAIL\n"); return 1; }
    game.GameType = 0;
    if (!Same(game.GetBestScore(), "")) { std::printf("007d6560_TEST FAIL\n"); return 1; }
    if (g_destroyed != 3 || g_def.m_count != 0) { std::printf("007d6560_TEST FAIL\n"); return 1; }
    std::printf("007d6560_TEST PASS\n");
    return 0;
}