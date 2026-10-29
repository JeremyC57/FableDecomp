#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
typedef int LONG32;   // the retail 4-byte long (the g++ host is LP64)
#define TEST_ADDR "006d3880"
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", TEST_ADDR, __LINE__); return 1; } } while (0)

class CCharString {
public:
    char m_text[64];
    static int s_live;
    CCharString() { m_text[0] = 0; ++s_live; }
    ~CCharString() { --s_live; }
    CCharString(const char* t) { std::strcpy(m_text, t); }
    const CCharString& operator+=(const char* text) { std::strcat(m_text, text); return *this; }
};
int CCharString::s_live = 0;

class CPersistContext { public: int id; };
static CPersistContext* g_lastContext = 0;
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, LONG32* value)
{
    char buf[16];
    std::sprintf(buf, "%d", (int)*value);
    g_lastContext = context;
    *out += buf;
}

namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
}
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    void TransferOut(CPersistContext& context, CCharString& out, const T* value) const;
};
template <> void CPersistTraits<LONG32>::TransferOut(CPersistContext& context, CCharString& out, const LONG32* value) const
{
    LONG32 copy = *value;
    TransferOutLong(&context, &out, &copy);
}
template <> void CPersistTraits<CCharString>::TransferOut(CPersistContext& context, CCharString& out, const CCharString* value) const
{
    out += "\"";
    out += value->m_text;
    out += "\"";
}
template <class T1, class T2> struct CPersistTraits<std::pair<T1, T2> > {
    void TransferOut(CPersistContext& context, CCharString& out, const std::pair<T1, T2>* value) const
    {
        CPersistTraits<T1> firstTraits;
        CPersistTraits<T2> secondTraits;
        out += "(";
        firstTraits.TransferOut(context, out, &value->first);
        out += ",";
        secondTraits.TransferOut(context, out, &value->second);
        out += ")";
    }
};

int main()
{
    CPersistContext c; CCharString s;
    std::pair<CCharString, LONG32> p; std::strcpy(p.first.m_text, "Name"); p.second = 7;
    int live = CCharString::s_live;
    CPersistTraits<std::pair<CCharString, LONG32> > t;
    t.TransferOut(c, s, &p);
    CHECK(std::strcmp(s.m_text, "(\"Name\",7)") == 0);
    CHECK(CCharString::s_live == live);   // the string traits' DefaultVal is built and destroyed
    std::printf("006d3880_TEST PASS\n");
    return 0;
}