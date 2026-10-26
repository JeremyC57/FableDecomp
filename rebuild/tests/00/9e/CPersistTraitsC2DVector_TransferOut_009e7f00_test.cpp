// Behaviour test for 0x009E7F00 CPersistTraits<C2DVector>::TransferOut: appends "C2DCoordF(x,y)" with six decimals.
// CCharString is a small fixed-buffer stand-in; FormatDouble formats with the requested digit count.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
class CPersistContext { public: int id; };

static int g_live = 0;
class CCharString { public:
    char Buf[128];
    CCharString() { Buf[0] = 0; ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(Buf, o.Buf); ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator+=(const char* text) { std::strcat(Buf, text); return *this; }
    CCharString& operator+=(const CCharString& other) { std::strcat(Buf, other.Buf); return *this; }
    static CCharString __fastcall FormatDouble(long digits, double value)
    {
        CCharString r; std::sprintf(r.Buf, "%.*f", (int)digits, value); return r;
    }
};

class C2DVector { public: float X; float Y; };

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, T* value);
};

template <>
void CPersistTraits<C2DVector>::TransferOut(CPersistContext* context, CCharString* out, C2DVector* value)
{
    *out += "C2DCoordF(";
    *out += CCharString::FormatDouble(6, value->X);
    *out += ",";
    *out += CCharString::FormatDouble(6, value->Y);
    *out += ")";
}

int main()
{
    CPersistTraits<C2DVector> traits;
    C2DVector v; v.X = 1.5f; v.Y = -2.25f;
    CCharString out; out += "P=";
    traits.TransferOut(0, &out, &v);
    if (std::strcmp(out.Buf, "P=C2DCoordF(1.500000,-2.250000)") != 0 || g_live != 1) {
        std::printf("009e7f00_TEST FAIL\n"); return 1;
    }
    std::printf("009e7f00_TEST PASS\n");
    return 0;
}