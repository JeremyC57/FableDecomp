// Behaviour test for 0x009E7F90 CPersistTraits<C3DVector>::TransferOut: appends "C3DCoordF(x,y,z)" with six decimals.
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
    const CCharString& operator+=(const char* text) { std::strcat(Buf, text); return *this; }
    const CCharString& operator+=(const CCharString& other) { std::strcat(Buf, other.Buf); return *this; }
    static CCharString __fastcall FormatDouble(long digits, double value)
    {
        CCharString r; std::sprintf(r.Buf, "%.*f", (int)digits, value); return r;
    }
};

class C3DVector { public: float X; float Y; float Z; };

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, T* value);
};

template <>
void CPersistTraits<C3DVector>::TransferOut(CPersistContext* context, CCharString* out, C3DVector* value)
{
    *out += "C3DCoordF(";
    *out += CCharString::FormatDouble(6, value->X);
    *out += ",";
    *out += CCharString::FormatDouble(6, value->Y);
    *out += ",";
    *out += CCharString::FormatDouble(6, value->Z);
    *out += ")";
}

int main()
{
    CPersistTraits<C3DVector> traits;
    C3DVector v; v.X = 1.5f; v.Y = -2.25f; v.Z = 100.0f;
    CCharString out; out += "P=";
    traits.TransferOut(0, &out, &v);
    if (std::strcmp(out.Buf, "P=C3DCoordF(1.500000,-2.250000,100.000000)") != 0 || g_live != 1) {
        std::printf("009e7f90_TEST FAIL\n"); return 1;
    }
    std::printf("009e7f90_TEST PASS\n");
    return 0;
}