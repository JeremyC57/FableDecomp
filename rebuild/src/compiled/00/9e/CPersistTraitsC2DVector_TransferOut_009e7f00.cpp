// CPersistTraits<C2DVector>::TransferOut(CPersistContext* context, CCharString* out, C2DVector* value) -- appends the vector as "C2DCoordF(x,y)" with six decimals per component
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E7F00, 130 bytes (+14 int3 padding), `ret 0Ch`.
// Callees: 0x0099F100 CCharString::operator+=(const char*), 0x009A08B0 CCharString::FormatDouble(long digits, double)
// (static __fastcall: hidden result in ecx, digits in edx, the double on the stack), 0x0099F0A0
// CCharString::operator+=(const CCharString&), 0x0099EAE0 ~CCharString. Strings "C2DCoordF(" @ 0x0129C238,
// "," @ 0x0122E024, ")" @ 0x0122E020.
// The text writer behind CPersistContext_TransferableComponent<C2DVector>::TransferOut (0x00800430). The context is
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

class C2DVector {
public:
    float X;
    float Y;
};

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