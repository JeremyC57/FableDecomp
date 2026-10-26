// CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferOut(CPersistContext* context, CCharString* out, const CUIMiniMapMarker* value) -- text persist write of a minimap marker as "(region,nameLength,name,x,y,graphic,angle)"
// Leaf: ?TransferOut@?$CPersistTraits@VCUIMiniMapMarker@CTCInventoryMap@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00614080, 361 bytes, `ret 0Ch` (traits `this` unused).
// Ego_r.pdb: ?TransferOut@?$CPersistTraits@VCUIMiniMapMarker@CTCInventoryMap@@@@QAEXAAVCPersistContext@@AAVCCharString@@
//   ABVCUIMiniMapMarker@CTCInventoryMap@@@Z; CUIMiniMapMarker (0x1C bytes) = { long Region +0, CCharString UIDefName +4,
//   C2DVector Position +8, long DefNameLength +0x10, long GraphicIndex +0x14, float Angle +0x18 }.
// Callees: CCharString() @ 0x0099E4B0, CCharString(const CCharString&) @ 0x0099EC30, ~CCharString @ 0x0099EAE0,
//   operator+=(const char*) @ 0x0099F100 ("(" 0x0122E028, "," 0x0122E024, ")" 0x0122E020), operator+=(const CCharString&)
//   @ 0x0099F0A0, CCharString::FormatDouble @ 0x009A08B0 (`__fastcall`, hidden result in ecx, six digits in edx, the double
//   on the stack), TransferOutLong @ 0x009E2560 (`__fastcall` context, out, &copy), CPersistTraits<CCharString>::TransferOut
//   @ 0x0040E630 (thiscall on the string traits, whose CCharString DefaultVal is built first and destroyed last).
// Shape: the landed 0x0057F78F / 0x0040FA20 text writers: each long through an inline CPersistTraits<long>::TransferOut
//   (copy into the dead `out` argument slot), each float through an inline CPersistTraits<float>::TransferOut (the
//   formatted temporary built in that slot after the 8-byte push), the name copied into a function-scope local that is
//   destroyed at the end before the traits. No EH frame (the TU has no /EHsc). Speed-optimised (default) TU.
class CPersistContext;
class CCharString {
public:
    class CCharStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator+=(const char* text);         // 0x0099F100
    CCharString& operator+=(const CCharString& other); // 0x0099F0A0
    static CCharString __fastcall FormatDouble(long digits, double value);   // 0x009A08B0
};
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560

class C2DVector {
public:
    float X;
    float Y;
};

class CTCInventoryMap {
public:
    class CUIMiniMapMarker {                           // 0x1C bytes
    public:
        long        Region;                            // +0x00
        CCharString UIDefName;                         // +0x04
        C2DVector   Position;                          // +0x08
        long        DefNameLength;                     // +0x10
        long        GraphicIndex;                      // +0x14
        float       Angle;                             // +0x18
    };
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<long> {
    __forceinline void TransferOut(CPersistContext* context, CCharString* out, const long* value) const
    {
        long copy = *value;
        TransferOutLong(context, out, &copy);
    }
};
template <> struct CPersistTraits<float> {
    __forceinline void TransferOut(CPersistContext* context, CCharString* out, const float* value) const
    {
        *out += CCharString::FormatDouble(6, *value);
    }
};
template <> struct CPersistTraits<CCharString> {
    CCharString DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, CCharString* value);   // 0x0040E630
};
template <> struct CPersistTraits<CTCInventoryMap::CUIMiniMapMarker> {
    void TransferOut(CPersistContext* context, CCharString* out, const CTCInventoryMap::CUIMiniMapMarker* value);
};

void CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferOut(CPersistContext* context, CCharString* out,
                                                                    const CTCInventoryMap::CUIMiniMapMarker* value)
{
    CPersistTraits<CCharString> stringTraits;
    CPersistTraits<long> longTraits;
    CPersistTraits<float> floatTraits;
    *out += "(";
    longTraits.TransferOut(context, out, &value->Region);
    *out += ",";
    longTraits.TransferOut(context, out, &value->DefNameLength);
    *out += ",";
    CCharString name(value->UIDefName);
    stringTraits.TransferOut(context, out, &name);
    *out += ",";
    floatTraits.TransferOut(context, out, &value->Position.X);
    *out += ",";
    floatTraits.TransferOut(context, out, &value->Position.Y);
    *out += ",";
    longTraits.TransferOut(context, out, &value->GraphicIndex);
    *out += ",";
    floatTraits.TransferOut(context, out, &value->Angle);
    *out += ")";
}