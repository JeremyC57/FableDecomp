// CPersistTraits<CCharString>::TransferOut(CPersistContext& context, CCharString& out, const CCharString& value) -- appends the string wrapped in double quotes
// Leaf: ?TransferOut@?$CPersistTraits@VCCharString@@@@QAEXAAVCPersistContext@@AAVCCharString@@ABV3@@Z
// Name (2026-10-31, hand621 NAME_FIXES): Ego_r.pdb 0x0040C2D0 / FableWin.pdb 0x0189E7E0: non-const member, references, the
//   value by const reference (was the pointer form). Bytes unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040E630, 118 bytes, `ret 0Ch`.
// Callees: 0x0099EBF0 CCharString::CCharString(const char*, long) on "" @ 0x0122D70E, 0x0099F600 operator+(const
// CCharString&, const char*) and 0x0099F570 operator+(const CCharString&, const CCharString&) (both __fastcall globals,
// hidden result in ecx, left operand in edx), 0x0099F0A0 CCharString::operator+=(const CCharString&), 0x0099EAE0
// ~CCharString (four temporaries). String "\"" @ 0x0122DF24.
// The element writer the landed CPersistContext_TransferableVector<CCharString>::TransferOut 0x0049B0B0 calls.
// Shape: `out += CCharString("") + "\"" + value + "\"";` -- VC evaluates the operands right to left (the closing
// quote, then `value`, then the opening quote are pushed before the "" constructor), the three temporaries chain
// through eax -> edx; the context is unused, so its argument slot holds the last temporary; the four temporaries die
// in reverse order of construction after the append.
class CPersistContext;

class CCharString { public:
    struct SBlock* m_pBlock;
    CCharString(const char* text, long length = -1);             // 0x0099EBF0
    ~CCharString();                                              // 0x0099EAE0
    const CCharString& operator+=(const CCharString& other);           // 0x0099F0A0
};

CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, const T& value);
};

template <>
void CPersistTraits<CCharString>::TransferOut(CPersistContext& context, CCharString& out, const CCharString& value)
{
    out += CCharString("") + "\"" + value + "\"";
}