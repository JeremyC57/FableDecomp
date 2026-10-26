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
#define TEST_ADDR "005cc6c0"
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

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, const T* value) const
    {
        LONG32 copy = *value;
        TransferOutLong(&context, &out, &copy);
    }
};
int main()
{
    CPersistContext c; CCharString s; LONG32 v = -42;
    CPersistTraits<LONG32> t;
    t.TransferOut(c, s, &v);
    CHECK(std::strcmp(s.m_text, "-42") == 0 && g_lastContext == &c && v == -42);
    std::printf("005cc6c0_TEST PASS\n");
    return 0;
}