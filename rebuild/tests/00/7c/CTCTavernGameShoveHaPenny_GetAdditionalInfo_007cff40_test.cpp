// behaviour test for 007cff40 CTCTavernGameShoveHaPenny::GetAdditionalInfo: "<text><current + 1>/<total>" from the tavern def's additional-info
// text, the game's counter and the sub-def's total; both defs are released (the tavern def through ReleaseTemporary)
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
#include <stddef.h>
static int g_destroyed = 0;
static long g_items[16];
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
};
struct CDefBase : CDefVirtual, CRefCounted {
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CTavernGameDef : CDefBase {
    unsigned long AdditionalInfo;
};
struct CSubDef : CDefBase {
    long Rounds;
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
struct CSubDefPointer {
    CSubDef* Object;
    CSubDefPointer() : Object(0) {}
    ~CSubDefPointer() { if (Object) Object->Release(); }
    CSubDef* operator->() const { return Object; }
};
static CTavernGameDef g_def;
static CSubDef g_sub;
struct CDefClassBase {
    bool GetSubDef(CSubDefPointer* out) { ++g_sub.m_count; out->Object = &g_sub; return true; }
};
struct CThing {
    CDefClassBase* DefClass;
};
struct CTCTavernGame {
    virtual void v00() {}
    CThing* Owner;
    void GetTavernGameDef(CDefPointer* out) { ++g_def.m_count; out->Object = &g_def; }
    CWideString GetText(unsigned long id) { char b[32]; std::sprintf(b, "I%lu:", id); return CWideString(b); }
    long RoundsPlayed;
};
struct CTCTavernGameShoveHaPenny : CTCTavernGame {
    CWideString GetAdditionalInfo();
};
CWideString CTCTavernGameShoveHaPenny::GetAdditionalInfo()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    CSubDefPointer shove;
    Owner->DefClass->GetSubDef(&shove);
    return GetText(def->AdditionalInfo) + GFIntToCharString(RoundsPlayed + 1).ToWideString() + L"/"
         + GFIntToCharString(shove->Rounds).ToWideString();
}
int main()
{
    CDefClassBase defClass; CThing owner; owner.DefClass = &defClass;
    g_def.AdditionalInfo = 5; g_def.m_count = 0; g_sub.m_count = 0;
    (&g_sub)->Rounds = 9;
    CTCTavernGameShoveHaPenny game; game.Owner = &owner; game.RoundsPlayed = 2;
    CWideString s = game.GetAdditionalInfo();
    if (!Same(s, "I5:3/9")) { std::printf("007cff40_TEST FAIL\n"); return 1; }
    if (g_destroyed != 2 || g_def.m_count != 0 || g_sub.m_count != 0) { std::printf("007cff40_TEST FAIL\n"); return 1; }
    std::printf("007cff40_TEST PASS\n");
    return 0;
}