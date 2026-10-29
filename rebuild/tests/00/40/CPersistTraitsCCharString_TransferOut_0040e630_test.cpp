// Behaviour test for 0x0040E630 CPersistTraits<CCharString>::TransferOut: appends the value wrapped in double quotes,
// built as CCharString("") + "\"" + value + "\"" (all temporaries destroyed afterwards).
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
class CPersistContext { public: int id; };

static int g_live = 0;
class CCharString { public:
    char Buf[128];
    CCharString(const char* text, long length = -1) { std::strcpy(Buf, text); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(Buf, o.Buf); ++g_live; }
    ~CCharString() { --g_live; }
    const CCharString& operator+=(const CCharString& other) { std::strcat(Buf, other.Buf); return *this; }
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right) { CCharString r(left); std::strcat(r.Buf, right.Buf); return r; }
CCharString __fastcall operator+(const CCharString& left, const char* right) { CCharString r(left); std::strcat(r.Buf, right); return r; }

template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal("") { }              // test-only: the stand-in string has no default constructor
    void TransferOut(CPersistContext* context, CCharString* out, T* value);
};

template <>
void CPersistTraits<CCharString>::TransferOut(CPersistContext* context, CCharString* out, CCharString* value)
{
    *out += CCharString("") + "\"" + *value + "\"";
}

int main()
{
    CCharString value("Bowerstone");
    CCharString out("Name=");
    CPersistTraits<CCharString> traits;
    traits.TransferOut(0, &out, &value);
    if (std::strcmp(out.Buf, "Name=\"Bowerstone\"") != 0 || g_live != 3 || traits.DefaultVal.Buf[0] != 0) {
        std::printf("0040e630_TEST FAIL\n"); return 1;
    }
    std::printf("0040e630_TEST PASS\n");
    return 0;
}