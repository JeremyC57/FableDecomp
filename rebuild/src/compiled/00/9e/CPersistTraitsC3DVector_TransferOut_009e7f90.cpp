// CPersistTraits<C3DVector>::TransferOut(CPersistContext* context, CCharString* out, C3DVector* value) -- appends the vector as "C3DCoordF(x,y,z)" with six decimals per component
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E7F90, 182 bytes, `ret 0Ch`.
// Callees: 0x0099F100 CCharString::operator+=(const char*), 0x009A08B0 CCharString::FormatDouble(long digits, double)
// (static __fastcall: hidden result in ecx, digits in edx, the double on the stack), 0x0099F0A0
// CCharString::operator+=(const CCharString&), 0x0099EAE0 ~CCharString. Strings "C3DCoordF(" @ 0x0129C244,
// "," @ 0x0122E024, ")" @ 0x0122E020.
// The C3DVector twin of CPersistTraits<C2DVector>::TransferOut 0x009E7F00 (same TU, next function). The context is
// unused, so its argument slot is dead and VC builds each FormatDouble temporary there (`lea ecx,[esp+18h]` after the
// 8-byte double push); `out` stays in esi and `value` in edi across the calls.
class CPersistContext;

class CCharString { public:
    struct SBlock* m_pBlock;
    ~CCharString();                                              // 0x0099EAE0
    CCharString& operator+=(const char* text);                   // 0x0099F100
    CCharString& operator+=(const CCharString& other);           // 0x0099F0A0
    static CCharString __fastcall FormatDouble(long digits, double value);   // 0x009A08B0
};

class C3DVector {
public:
    float X;
    float Y;
    float Z;
};

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