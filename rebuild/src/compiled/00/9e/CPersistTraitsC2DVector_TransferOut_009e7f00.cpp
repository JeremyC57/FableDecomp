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
    const CCharString& operator+=(const char* text);                   // 0x0099F100
    const CCharString& operator+=(const CCharString& other);           // 0x0099F0A0
};
// 0x009A08B0: Ego_r.pdb + FableWin.pdb ?GFFloatToCharString@@YI?AVCCharString@@NJ@Z (NF94, q11248; was the stand-in static CCharString::FormatDouble)
CCharString __fastcall GFFloatToCharString(double n, long precision);

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
    *out += GFFloatToCharString(value->X, 6);
    *out += ",";
    *out += GFFloatToCharString(value->Y, 6);
    *out += ")";
}