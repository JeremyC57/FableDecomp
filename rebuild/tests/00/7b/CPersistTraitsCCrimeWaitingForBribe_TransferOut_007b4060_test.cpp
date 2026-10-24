// Behaviour test for 0x007B4060 CPersistTraits<CCrimeWaitingForBribe>::TransferOut: appends "(deedHandle,type)".
// CCharString is a small fixed-buffer stand-in; GFIntToCharString formats a long.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
struct CPersistContext { int id; };

static int g_live = 0;
struct CCharString {
    char Buf[128];
    CCharString() { Buf[0] = 0; ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(Buf, o.Buf); ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator+=(const char* text) { std::strcat(Buf, text); return *this; }
    CCharString& operator+=(const CCharString& other) { std::strcat(Buf, other.Buf); return *this; }
};
CCharString __fastcall GFIntToCharString(long value) { CCharString r; std::sprintf(r.Buf, "%ld", value); return r; }

struct CCrimeWaitingForBribe { long DeedHandle; long Type; };

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, T* value);
};

template <>
void CPersistTraits<CCrimeWaitingForBribe>::TransferOut(CPersistContext* context, CCharString* out, CCrimeWaitingForBribe* value)
{
    *out += "(";
    *out += GFIntToCharString(value->DeedHandle);
    *out += ",";
    *out += GFIntToCharString(value->Type);
    *out += ")";
}

int main()
{
    CPersistTraits<CCrimeWaitingForBribe> traits;
    CCrimeWaitingForBribe v; v.DeedHandle = 1234; v.Type = -3;
    CCharString out; out += "P=";
    traits.TransferOut(0, &out, &v);
    if (std::strcmp(out.Buf, "P=(1234,-3)") != 0 || g_live != 1) {
        std::printf("007b4060_TEST FAIL\n"); return 1;
    }
    std::printf("007b4060_TEST PASS\n");
    return 0;
}