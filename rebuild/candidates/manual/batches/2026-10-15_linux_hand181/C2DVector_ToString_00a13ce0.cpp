// C2DVector::ToString() const -- format the vector as "(X, Y)" with two decimals
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A13CE0, length 188 bytes (`ret 4`, CCharString by hidden pointer).
// Class: the catalog's C3DVector::ToString label is a BSim match (0.84); the body formats only X (+0) and Y (+4), and
// the near-identical neighbour 0x00A13DA0 is the real C3DVector::ToString (0.9999), so this is C2DVector::ToString
// (?ToString@C2DVector@@QBE?AVCCharString@@XZ in Ego_r.pdb).
// Callees: CCharString::FormatDouble(long digits, double) @ 0x009A08B0 (static __fastcall: result in ecx, digits in
// edx), CCharString::CCharString(const char*, long) @ 0x0099EBF0, operator+(const CCharString&, const CCharString&)
// @ 0x0099F570 and operator+(const CCharString&, const char*) @ 0x0099F600 (both __fastcall globals, result in ecx,
// left in edx), ~CCharString @ 0x0099EAE0 (six temporaries). Strings "(" @ 0x0122E028, ", " @ 0x012397CC, ")" @ 0x0122E020.
// Shape: `CCharString("(") + FormatDouble(2, X) + ", " + FormatDouble(2, Y) + ")"` -- VC evaluates the operands right to
// left (")" pushed first, then Y, then X), the final operator+ writes straight into the hidden result.
struct CCharString
{
    void* PStringData;
    CCharString(const char* text, long length = -1);                         // 0x0099EBF0
    ~CCharString();                                                          // 0x0099EAE0
    static CCharString __fastcall FormatDouble(long digits, double value);   // 0x009A08B0
};

CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600

struct C2DVector
{
    float X, Y;
    CCharString ToString() const;
};

CCharString C2DVector::ToString() const
{
    return CCharString("(") + CCharString::FormatDouble(2, X) + ", " + CCharString::FormatDouble(2, Y) + ")";
}
