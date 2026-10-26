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
#define TEST_ADDR "00755a30"
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", TEST_ADDR, __LINE__); return 1; } } while (0)

class CCharString {
public:
    char m_text[64];
    CCharString() { m_text[0] = 0; }
    CCharString(const char* t) { std::strcpy(m_text, t); }
    CCharString& operator+=(const char* text) { std::strcat(m_text, text); return *this; }
};

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
    std::pair<LONG32, LONG32> p; p.first = 3; p.second = -9;
    CPersistTraits<std::pair<LONG32, LONG32> > t;
    t.TransferOut(c, s, &p);
    CHECK(std::strcmp(s.m_text, "(3,-9)") == 0);
    std::printf("00755a30_TEST PASS\n");
    return 0;
}