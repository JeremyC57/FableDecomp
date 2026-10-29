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
#define TEST_ADDR "006c78f0"
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", TEST_ADDR, __LINE__); return 1; } } while (0)

class CCharString {
public:
    char m_text[64];
    CCharString() { m_text[0] = 0; }
    CCharString(const char* t) { std::strcpy(m_text, t); }
    const CCharString& operator+=(const char* text) { std::strcat(m_text, text); return *this; }
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
class COpinionDeed { public: int m_id; };
static const void* g_deedTraitsThis = 0;
template <> void CPersistTraits<COpinionDeed*>::TransferOut(CPersistContext& context, CCharString& out, COpinionDeed* const* value) const
{
    char buf[16];
    g_deedTraitsThis = this;
    std::sprintf(buf, "deed%d", (*value)->m_id);
    out += buf;
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
    COpinionDeed deed; deed.m_id = 5;
    std::pair<LONG32, COpinionDeed*> p; p.first = 12; p.second = &deed;
    CPersistTraits<std::pair<LONG32, COpinionDeed*> > t;
    t.TransferOut(c, s, &p);
    CHECK(std::strcmp(s.m_text, "(12,deed5)") == 0 && g_deedTraitsThis != 0);
    std::printf("006c78f0_TEST PASS\n");
    return 0;
}