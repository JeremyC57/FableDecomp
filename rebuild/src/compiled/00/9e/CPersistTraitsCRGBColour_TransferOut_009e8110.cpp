// CPersistTraits<CRGBColour>::TransferOut(CPersistContext*, CCharString*, const CRGBColour*) -- append "CRGBColour(R,G,B,A)" to the text save
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E8110, 194 bytes (+ int3 padding), `ret 0Ch` (__fastcall, the empty traits `this` is unused).
// Callees: CCharString::operator+=(const char*) @ 0x0099F100, GFIntToCharString(long) @ 0x0099F830 (__fastcall,
//   hidden result in ecx, value in edx), CCharString::operator+=(const CCharString&) @ 0x0099F0A0, ~CCharString
//   @ 0x0099EAE0. Strings: "CRGBColour(" 0x0129C25C, "," 0x0122E024, ")" 0x0122E020.
// Layout: PDB CRGBColour = union { struct { uchar B, G, R, A; }; ... } (D3DCOLOR byte order), so R is +2, G +1, B +0,
//   A +3. Local model of the channel view.
// Shape: nine appends in source order; each channel goes through a GFIntToCharString temporary (homed in the dead
//   context-argument slot) destroyed right after its append. Speed-optimised (default) TU.
class CCharStringData;
class CPersistContext;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator+=(const char* text);          // 0x0099F100
    const CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
};
CCharString __fastcall GFIntToCharString(long value);   // 0x0099F830

class CRGBColour { public:                                     // the union's channel view
    unsigned char B, G, R, A;
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CRGBColour> {
    void TransferOut(CPersistContext* context, CCharString* out, const CRGBColour* value);
};

void CPersistTraits<CRGBColour>::TransferOut(CPersistContext* context, CCharString* out, const CRGBColour* value)
{
    *out += "CRGBColour(";
    *out += GFIntToCharString(value->R);
    *out += ",";
    *out += GFIntToCharString(value->G);
    *out += ",";
    *out += GFIntToCharString(value->B);
    *out += ",";
    *out += GFIntToCharString(value->A);
    *out += ")";
}